#pragma once
#include "Piece.hpp"

class Rook : public Piece
{
public:
    Rook(Square pos, const sf::Texture& texture, bool black)
        : Piece(pos, texture,black)
    {
        color = sf::Color(255, 165, 0);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};