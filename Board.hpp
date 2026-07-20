#pragma once

#include <vector>
#include <memory>
#include "Piece.hpp"

class Board
{
public:
    Board();

    void draw(sf::RenderWindow& window);
    std::vector<std::unique_ptr<Piece>>& getPieces();
    void set_potential_moves(const std::vector<Square>& new_moves);

    std::vector<Square> getSlidingMoves(Piece& piece, bool careAboutCheck);
    std::vector<Square> getNormalMoves(Piece& piece, bool careAboutCheck);
    std::vector<Square> getPawnMoves(Piece& piece, bool careAboutCheck);
    std::vector<Square> getMoves(Piece& piece, bool careAboutCheck);

    bool inBounds(Square& current);
    Piece* getPieceAt(Square current);
    void removePieceAt(Square square);

    bool isLegalMove(Square& clicked, std::vector<Square> moves);
    bool movePiece(Board& board, Square& clicked, bool blackTurn);
    
    bool testMoveForCheck(Piece& piece, Square& dir);
    bool checkForCheck(bool isBlack);

private:
    sf::Texture texture;
    std::vector<std::unique_ptr<Piece>> pieces;
    std::vector<Square> potential_moves;
};