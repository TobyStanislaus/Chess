#pragma once
#include "Board.hpp"
#include <algorithm>

constexpr int NUM_PLANES = 17;
constexpr int BOARD_SIZE = 8;
constexpr int TENSOR_SIZE = NUM_PLANES * BOARD_SIZE * BOARD_SIZE; // 1088

// Matches Python's PIECE_TYPES = [Pawn, Knight, Bishop, Rook, Queen, King] ordering exactly.
// NOTE: this is intentionally NOT the same order as the PieceType enum's underlying values.
inline int pieceTypeToPlaneIndex(PieceType type) {
    switch (type) {
        case PieceType::Pawn:   return 0;
        case PieceType::Knight: return 1;
        case PieceType::Bishop: return 2;
        case PieceType::Rook:   return 3;
        case PieceType::Queen:  return 4;
        case PieceType::King:   return 5;
    }
    return -1; // unreachable
}

// Mirrors python-chess's has_kingside/queenside_castling_rights():
// only checks king/rook haven't moved, NOT whether castling is legal right now.
inline bool hasCastlingRights(Board& board, bool black, bool kingside) {
    int row = black ? 0 : 7;

    Piece* king = board.getPieceAt(Square{row, 4});
    if (!king || king->getType() != PieceType::King || king->getBlack() != black || !king->getFirstMove())
        return false;

    int rookCol = kingside ? 7 : 0;
    Piece* rook = board.getPieceAt(Square{row, rookCol});
    if (!rook || rook->getType() != PieceType::Rook || rook->getBlack() != black || !rook->getFirstMove())
        return false;

    return true;
}

// Fills a pre-allocated float array of size TENSOR_SIZE (1088), matching the Python
// encode_board() layout exactly.
inline void encodeBoard(Board& board, bool blackTurn, float* output) {
    std::fill(output, output + TENSOR_SIZE, 0.0f);

    auto idx = [](int plane, int row, int col) {
        return plane * BOARD_SIZE * BOARD_SIZE + row * BOARD_SIZE + col;
    };

    for (auto& piece : board.getPieces()) {
        int planeBase = pieceTypeToPlaneIndex(piece->getType());
        int plane = piece->getBlack() ? planeBase + 6 : planeBase;

        Square pos = piece->getPosition();
        output[idx(plane, pos.row, pos.col)] = 1.0f;
    }

    if (!blackTurn) {
        for (int r = 0; r < BOARD_SIZE; r++)
            for (int c = 0; c < BOARD_SIZE; c++)
                output[idx(12, r, c)] = 1.0f;
    }

    bool whiteKingside  = hasCastlingRights(board, false, true);
    bool whiteQueenside = hasCastlingRights(board, false, false);
    bool blackKingside  = hasCastlingRights(board, true,  true);
    bool blackQueenside = hasCastlingRights(board, true,  false);

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (whiteKingside)  output[idx(13, r, c)] = 1.0f;
            if (whiteQueenside) output[idx(14, r, c)] = 1.0f;
            if (blackKingside)  output[idx(15, r, c)] = 1.0f;
            if (blackQueenside) output[idx(16, r, c)] = 1.0f;
        }
    }
}