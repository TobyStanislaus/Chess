#pragma once
#include "pieces/Piece.hpp"
#include <array>
#include <algorithm>
#include <cmath>

constexpr int NUM_MOVE_TYPES = 73;
constexpr int POLICY_SIZE = 64 * NUM_MOVE_TYPES; // 4672

// Must match move_encoding.py's QUEEN_DIRECTIONS/KNIGHT_DELTAS exactly -
// expressed in python-chess's (delta_rank, delta_file), where +rank = toward
// Black's back rank (i.e. "up the board" from White's own perspective).
inline const std::array<std::pair<int,int>, 8> QUEEN_DIRECTIONS = {{
    {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}
}};
inline const std::array<std::pair<int,int>, 8> KNIGHT_DELTAS = {{
    {2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}
}};

inline int underpromoPieceIndex(PieceType type) {
    switch (type) {
        case PieceType::Knight: return 0;
        case PieceType::Bishop: return 1;
        case PieceType::Rook:   return 2;
        default: return -1; // Queen isn't encoded here - see move_encoding.py
    }
}

// row 0 = rank 8 (our convention) -> python-chess square index (a1=0, h8=63)
inline int toPythonSquare(Square s) {
    int rank = 7 - s.row;
    int file = s.col;
    return rank * 8 + file;
}

// Mirrors move_to_index() in move_encoding.py. Returns -1 if something about
// the move doesn't match any known pattern (shouldn't happen for legal moves).
inline int moveToIndex(const Move& move) {
    int fromSq = toPythonSquare(move.from);

    // python dr/df: dr = -(our row delta), df = our col delta (col == file, no flip needed)
    int drow = move.to.row - move.from.row;
    int dcol = move.to.col - move.from.col;
    int dr = -drow;
    int df = dcol;

    if (move.isPromotion && move.promotionPiece != PieceType::Queen) {
        // rel = (1, df) - direction is normalized to ignore color, per move_encoding.py
        int dirIndex = df + 1; // df in {-1,0,1} -> 0,1,2
        int plane = 64 + dirIndex * 3 + underpromoPieceIndex(move.promotionPiece);
        return fromSq * NUM_MOVE_TYPES + plane;
    }

    int absDr = std::abs(dr), absDf = std::abs(df);
    if ((absDr == 2 && absDf == 1) || (absDr == 1 && absDf == 2)) {
        for (int i = 0; i < 8; i++)
            if (KNIGHT_DELTAS[i] == std::make_pair(dr, df))
                return fromSq * NUM_MOVE_TYPES + 56 + i;
    }

    int distance = std::max(absDr, absDf);
    int stepR = dr / distance, stepF = df / distance;
    for (int i = 0; i < 8; i++)
        if (QUEEN_DIRECTIONS[i] == std::make_pair(stepR, stepF))
            return fromSq * NUM_MOVE_TYPES + i * 7 + (distance - 1);

    return -1; // unreachable for a legal move
}