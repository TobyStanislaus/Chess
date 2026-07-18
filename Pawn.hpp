#pragma once
#include "Piece.hpp"

class Pawn : public Piece
{
public:
    Pawn(Square pos, const sf::Texture& texture)
        : Piece(pos, texture)
    {
        color = sf::Color::Green;
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};