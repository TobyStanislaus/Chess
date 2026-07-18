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
    bool inBounds(Square& current);
    bool friendlyPiece(Square current);
private:
    std::vector<std::unique_ptr<Piece>> pieces;
    std::vector<Square> potential_moves;
};