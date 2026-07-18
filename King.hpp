#pragma once
#include "Piece.hpp"

class King : public Piece
{
public:
    King(Square pos, const sf::Texture& texture)
        : Piece(pos, texture)
    {
        color = sf::Color::Cyan;
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};