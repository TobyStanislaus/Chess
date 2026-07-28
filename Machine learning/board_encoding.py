"""
Converts a python-chess Board into a tensor suitable for a neural network.

Encoding (17 planes of 8x8), NOT perspective-flipped yet (see note at bottom):
  planes 0-5   : White piece positions (Pawn, Knight, Bishop, Rook, Queen, King)
  planes 6-11  : Black piece positions (same order)
  plane 12     : side to move (all 1s if White to move, all 0s if Black to move)
  planes 13-16 : castling rights (White kingside, White queenside, Black kingside, Black queenside)

This is a simplified first version. Real AlphaZero also encodes move history
(last 8 positions) and flips the board to always be from the perspective of
the player to move. We're starting simple to get something correct and
testable; both of those are natural follow-ups once this is verified working.
"""

import numpy as np
import chess

PIECE_TYPES = [chess.PAWN, chess.KNIGHT, chess.BISHOP, chess.ROOK, chess.QUEEN, chess.KING]
NUM_PLANES = 17


def encode_board(board: chess.Board) -> np.ndarray:
    """Returns a (17, 8, 8) float32 tensor representing the given position."""
    planes = np.zeros((NUM_PLANES, 8, 8), dtype=np.float32)

    for piece_type in PIECE_TYPES:
        for square in board.pieces(piece_type, chess.WHITE):
            row, col = square_to_row_col(square)
            planes[PIECE_TYPES.index(piece_type)][row][col] = 1.0

        for square in board.pieces(piece_type, chess.BLACK):
            row, col = square_to_row_col(square)
            planes[6 + PIECE_TYPES.index(piece_type)][row][col] = 1.0

    if board.turn == chess.WHITE:
        planes[12][:][:] = 1.0

    planes[13][:][:] = 1.0 if board.has_kingside_castling_rights(chess.WHITE) else 0.0
    planes[14][:][:] = 1.0 if board.has_queenside_castling_rights(chess.WHITE) else 0.0
    planes[15][:][:] = 1.0 if board.has_kingside_castling_rights(chess.BLACK) else 0.0
    planes[16][:][:] = 1.0 if board.has_queenside_castling_rights(chess.BLACK) else 0.0

    return planes


def square_to_row_col(square: chess.Square):
    """python-chess squares are 0-63, a1=0, h8=63. We map row 0 = rank 8 (top of board)."""
    row = 7 - chess.square_rank(square)
    col = chess.square_file(square)
    return row, col
