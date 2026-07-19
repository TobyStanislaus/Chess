#pragma once
#include "Piece.hpp"

class Bishop : public Piece
{
public:
    Bishop(Square pos, const sf::Texture& texture,bool black)
        : Piece(pos, texture,black)
    {
        color = sf::Color::Black;
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};