"""
Encodes chess moves as indices into a fixed-size policy vector, using the
scheme from the AlphaZero paper: for each of the 64 origin squares, there are
73 possible "move types":

  0-55  : "queen-like" moves - 8 directions x 7 distances (1-7 squares)
  56-63 : knight moves (8 possible L-shapes)
  64-72 : underpromotions - 3 directions (capture-left, straight, capture-right)
          x 3 piece types (knight, bishop, rook)

Queen promotions are NOT a separate case - they're encoded as a normal
1-square diagonal/straight pawn move (a queen-like move of distance 1), and
we infer "this must be a queen promotion" from board context (a pawn moving
to the back rank) when decoding. This matches the original AlphaZero scheme.

Total policy size = 64 origin squares * 73 move types = 4672.
"""

import chess

NUM_MOVE_TYPES = 73
POLICY_SIZE = 64 * NUM_MOVE_TYPES

# 8 directions, clockwise from "up the board" (increasing rank), as (delta_rank, delta_file)
QUEEN_DIRECTIONS = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]

KNIGHT_DELTAS = [(2, 1), (1, 2), (-1, 2), (-2, 1), (-2, -1), (-1, -2), (1, -2), (2, -1)]

# Underpromotion: 3 directions (matches "forward" and the two diagonal captures)
UNDERPROMO_DIRECTIONS = [(1, -1), (1, 0), (1, 1)]  # relative to White moving up; mirrored for Black
UNDERPROMO_PIECES = [chess.KNIGHT, chess.BISHOP, chess.ROOK]


def _delta(from_sq, to_sq):
    return (chess.square_rank(to_sq) - chess.square_rank(from_sq),
            chess.square_file(to_sq) - chess.square_file(from_sq))


def move_to_index(move: chess.Move) -> int:
    """Maps a chess.Move to a unique index in [0, POLICY_SIZE)."""
    from_sq = move.from_square
    dr, df = _delta(from_sq, move.to_square)

    if move.promotion in (chess.KNIGHT, chess.BISHOP, chess.ROOK):
        # Underpromotion. Direction sign depends on which side is promoting;
        # normalize so "dr" is always +1 in the direction of travel.
        step_df = df if dr != 0 else 0
        # dr is already +1 or -1 depending on color; normalize direction index
        direction = (1 if dr > 0 else -1, step_df)
        # collapse to forward/left/right regardless of color
        rel = (1, step_df)
        plane = 64 + UNDERPROMO_DIRECTIONS.index(rel) * 3 + UNDERPROMO_PIECES.index(move.promotion)
        return from_sq * NUM_MOVE_TYPES + plane

    if (abs(dr), abs(df)) in [(2, 1), (1, 2)]:
        plane = 56 + KNIGHT_DELTAS.index((dr, df))
        return from_sq * NUM_MOVE_TYPES + plane

    # Queen-like move (includes queen promotions and all sliding/king/pawn-forward moves)
    distance = max(abs(dr), abs(df))
    direction = (dr // distance, df // distance)
    plane = QUEEN_DIRECTIONS.index(direction) * 7 + (distance - 1)
    return from_sq * NUM_MOVE_TYPES + plane


def index_to_move(index: int, board: chess.Board) -> chess.Move:
    """
    Maps a policy index back to a chess.Move, given the current board (needed
    to know whether a pawn reaching the back rank implies a queen promotion).
    Does NOT check legality - callers should verify against board.legal_moves.
    """
    from_sq = index // NUM_MOVE_TYPES
    plane = index % NUM_MOVE_TYPES

    if plane < 56:
        direction = QUEEN_DIRECTIONS[plane // 7]
        distance = (plane % 7) + 1
        to_sq = chess.square(
            chess.square_file(from_sq) + direction[1] * distance,
            chess.square_rank(from_sq) + direction[0] * distance,
        )
        promotion = None
        piece = board.piece_at(from_sq)
        if piece and piece.piece_type == chess.PAWN and chess.square_rank(to_sq) in (0, 7):
            promotion = chess.QUEEN
        return chess.Move(from_sq, to_sq, promotion=promotion)

    if plane < 64:
        dr, df = KNIGHT_DELTAS[plane - 56]
        to_sq = chess.square(chess.square_file(from_sq) + df, chess.square_rank(from_sq) + dr)
        return chess.Move(from_sq, to_sq)

    sub = plane - 64
    direction = UNDERPROMO_DIRECTIONS[sub // 3]
    promo_piece = UNDERPROMO_PIECES[sub % 3]
    piece = board.piece_at(from_sq)
    dr = direction[0] if (piece and piece.color == chess.WHITE) else -direction[0]
    df = direction[1] if (piece and piece.color == chess.WHITE) else -direction[1]
    to_sq = chess.square(chess.square_file(from_sq) + df, chess.square_rank(from_sq) + dr)
    return chess.Move(from_sq, to_sq, promotion=promo_piece)
