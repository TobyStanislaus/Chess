#pragma once
#include "Piece.hpp"

class Queen : public Piece
{
public:
    Queen(Square pos, const sf::Texture& texture)
        : Piece(pos, texture)
    {
        color = sf::Color::Magenta;
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};