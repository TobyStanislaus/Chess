"""
Plays full self-play games using MCTS + the current network, recording
training data for every position visited: (board tensor, search-improved
policy, final game outcome).

This is where the actual "improvement loop" lives: MCTS uses the network to
guide search, search produces a better policy than the network's raw output,
and we record that improved policy as a training target. Once the network is
trained on enough of these games, its raw output gets a little closer to
"what search would have found," which lets the NEXT round of self-play search
even more effectively. Repeat.
"""

import numpy as np
import chess

from board_encoding import encode_board
from move_encoding import move_to_index, POLICY_SIZE
from mcts import run_mcts, get_visit_count_policy, select_move


def play_one_game(network, num_simulations: int = 100, temperature_moves: int = 30,
                   max_moves: int = 200):
    """
    Plays one full self-play game. Returns a list of training examples:
    [(board_tensor, policy_target, outcome), ...] - one entry per position
    visited during the game.

    temperature_moves: for the first N plies, sample moves proportionally to
    visit count (temperature=1) to encourage varied openings; after that,
    play the most-visited move deterministically (temperature=0), since by
    the midgame we want the strongest move, not more exploration.
    """
    board = chess.Board()

    # We don't know the game's outcome until it ends, so we store
    # (encoded_board, policy_target, side_to_move) now and fill in the actual
    # outcome (+1 / -1 / 0 from each position's side-to-move perspective)
    # once the game is over.
    history = []

    move_count = 0
    while not board.is_game_over() and move_count < max_moves:
        root = run_mcts(board, network, num_simulations=num_simulations, add_noise=True)

        visit_counts = get_visit_count_policy(root)
        total_visits = sum(visit_counts.values())

        # Build the full 4672-length policy target vector (mostly zeros,
        # nonzero only at legal moves that were actually explored)
        policy_target = np.zeros(POLICY_SIZE, dtype=np.float32)
        for move, count in visit_counts.items():
            policy_target[move_to_index(move)] = count / total_visits

        encoded = encode_board(board)
        history.append([encoded, policy_target, board.turn])

        temperature = 1.0 if move_count < temperature_moves else 0.0
        move = select_move(root, temperature=temperature)
        board.push(move)
        move_count += 1

    # Determine the game outcome from White's perspective: +1 White win,
    # -1 Black win, 0 draw (includes hitting max_moves without a result)
    if board.is_checkmate():
        # The side to move is the one who got checkmated (they have no moves)
        white_outcome = -1.0 if board.turn == chess.WHITE else 1.0
    else:
        white_outcome = 0.0  # stalemate, insufficient material, repetition, move limit, etc.

    # Convert to each position's own side-to-move perspective: value should
    # represent "was this good for whoever was about to move here"
    training_examples = []
    for encoded, policy_target, side_to_move in history:
        outcome = white_outcome if side_to_move == chess.WHITE else -white_outcome
        training_examples.append((encoded, policy_target, outcome))

    return training_examples, board.result()