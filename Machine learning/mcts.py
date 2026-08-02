"""
Monte Carlo Tree Search guided by a policy/value network (AlphaZero-style).

Core idea: rather than trusting the network's raw policy output directly, we
run many simulated playouts through a search tree. Each simulation:
  1. SELECT: walk down the tree from the root, at each node picking the child
     that maximizes a UCB-style score balancing "looks good" (Q) against
     "under-explored" (U, driven by the network's prior + visit counts).
  2. EXPAND: when we reach a node we haven't visited before, ask the network
     for its policy (move priors) and value estimate, and create child nodes
     for every legal move.
  3. BACKUP: propagate the leaf's value estimate back up the path we took,
     updating visit counts and accumulated value at every node along the way.

After many simulations, the *visit counts* at the root (not the network's raw
policy) become the improved policy - this is the actual training target.
This is the core mechanism that lets self-play bootstrap a network into
playing better than its own raw policy output.
"""

import math
import numpy as np
import chess
import torch

from board_encoding import encode_board
from move_encoding import move_to_index, POLICY_SIZE

# Exploration constant balancing "trust the network's prior" vs "explore
# under-visited moves". Higher = more exploration. 1.5-2.5 is typical.
C_PUCT = 1.5


class Node:
    def __init__(self, prior: float):
        self.prior = prior            # P(s,a) - network's prior probability for the move that led here
        self.visit_count = 0          # N(s,a)
        self.value_sum = 0.0          # total backed-up value, accumulated
        self.children = {}            # chess.Move -> Node
        self.to_move = None           # chess.WHITE / chess.BLACK - whose turn at this node

    def expanded(self) -> bool:
        return len(self.children) > 0

    def value(self) -> float:
        """Mean value Q(s,a) from this node's perspective. 0 if unvisited."""
        if self.visit_count == 0:
            return 0.0
        return self.value_sum / self.visit_count


def _ucb_score(parent: Node, child: Node) -> float:
    """
    UCB = Q(s,a) + U(s,a)
    Q is "how good has this move looked so far" (from the parent's perspective,
    hence the sign flip - a child node's value is from the *other* player's
    perspective, since it's their turn to move there).
    U rewards moves the network liked (high prior) that haven't been tried
    much yet (low visit count relative to the parent's total visits).
    """
    u = C_PUCT * child.prior * math.sqrt(parent.visit_count) / (1 + child.visit_count)
    q = -child.value()  # flip sign: child's value is from the opponent's perspective
    return q + u


def _select_child(node: Node):
    """Pick the child maximizing UCB score. Returns (move, child_node)."""
    best_score = -float('inf')
    best_move = None
    best_child = None
    for move, child in node.children.items():
        score = _ucb_score(node, child)
        if score > best_score:
            best_score = score
            best_move = move
            best_child = child
    return best_move, best_child


@torch.no_grad()
def _evaluate(node: Node, board: chess.Board, network) -> float:
    """
    Runs the network on `board`, expands `node` with a child for every legal
    move (using the network's policy as priors, masked+renormalized over
    legal moves only), and returns the network's value estimate for `board`
    from the perspective of the player to move.
    """
    node.to_move = board.turn

    if board.is_game_over():
        # Terminal node - no children to expand. Value from the perspective
        # of the player to move: -1 if they've been checkmated, 0 otherwise
        # (stalemate / other draws).
        if board.is_checkmate():
            return -1.0
        return 0.0

    encoded = encode_board(board)
    tensor = torch.from_numpy(encoded).unsqueeze(0)
    policy_logits, value = network(tensor)

    policy_logits = policy_logits[0].numpy()
    legal_moves = list(board.legal_moves)

    # Mask illegal moves: the network outputs a score for all 4672 possible
    # (from_square, move_type) combinations, but only a handful are legal in
    # any given position. We must restrict to legal moves before turning
    # scores into probabilities - softmaxing over all 4672 would let the
    # network split probability mass onto moves that don't exist here.
    legal_indices = [move_to_index(m) for m in legal_moves]
    legal_logits = policy_logits[legal_indices]

    # Softmax over legal moves only
    legal_logits = legal_logits - legal_logits.max()  # numerical stability
    exp_logits = np.exp(legal_logits)
    priors = exp_logits / exp_logits.sum()

    for move, prior in zip(legal_moves, priors):
        node.children[move] = Node(prior=float(prior))

    return float(value.item())


def _add_root_noise(root: Node, alpha: float = 0.3, frac: float = 0.25):
    """
    Mixes Dirichlet noise into the root's children's priors. Only ever applied
    at the root, never deeper in the tree - the point is to occasionally push
    self-play into trying a move the network wouldn't normally favor, so
    training data covers more of the game tree than the network's current
    (possibly narrow, possibly wrong) preferences. Without this, self-play
    games would be far too repetitive, especially early when the network's
    priors are close to random anyway but still slightly, arbitrarily biased.
    `alpha` controls how "spread out" the noise is (lower = spikier/more
    concentrated on fewer moves); `frac` controls how much noise vs. original
    prior is blended in (0.25 = 75% original prior, 25% noise).
    """
    moves = list(root.children.keys())
    noise = np.random.dirichlet([alpha] * len(moves))
    for move, n in zip(moves, noise):
        child = root.children[move]
        child.prior = child.prior * (1 - frac) + n * frac


def run_mcts(root_board: chess.Board, network, num_simulations: int = 100,
             add_noise: bool = False) -> Node:
    """
    Runs MCTS from the given position and returns the root node, whose
    children's visit_counts represent the search-improved policy.

    add_noise should be True during self-play (encourages exploration) and
    False during actual/competitive play (always trust the network+search).
    """
    root = Node(prior=1.0)
    _evaluate(root, root_board, network)

    if add_noise:
        _add_root_noise(root)

    for _ in range(num_simulations):
        node = root
        board = root_board.copy()
        path = [node]

        # SELECT: descend until we hit a node with no children yet
        while node.expanded():
            move, node = _select_child(node)
            board.push(move)
            path.append(node)

        # EXPAND + evaluate the leaf we landed on
        value = _evaluate(node, board, network)

        # BACKUP: propagate value up the path, flipping sign at each step
        # since each node up the path represents the *other* player's turn
        for visited_node in reversed(path):
            visited_node.value_sum += value
            visited_node.visit_count += 1
            value = -value

    return root


def get_visit_count_policy(root: Node) -> dict:
    """Returns {move: visit_count} for the root's children - the search-improved policy."""
    return {move: child.visit_count for move, child in root.children.items()}


def select_move(root: Node, temperature: float = 1.0):
    """
    Picks a move from the root's children based on visit counts.
    temperature=0 -> always pick the most-visited move (used for actual play).
    temperature=1 -> sample proportionally to visit counts (used during
    self-play training to encourage exploration/move diversity).
    """
    moves = list(root.children.keys())
    visit_counts = np.array([root.children[m].visit_count for m in moves], dtype=np.float64)

    if temperature == 0:
        return moves[int(np.argmax(visit_counts))]

    # Sharpen or flatten the distribution by temperature, then sample
    weighted = visit_counts ** (1.0 / temperature)
    probs = weighted / weighted.sum()
    return np.random.choice(moves, p=probs)