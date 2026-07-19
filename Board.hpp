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

    std::vector<Square> getSlidingMoves(Piece& piece);
    std::vector<Square> getNormalMoves(Piece& piece);
    std::vector<Square> getPawnMoves(Piece& piece);
    std::vector<Square> getMoves(Piece& piece);

    bool inBounds(Square& current);
    Piece* getPieceAt(Square current);
    void removePieceAt(Square square);
    
private:
    std::vector<std::unique_ptr<Piece>> pieces;
    std::vector<Square> potential_moves;
};