#pragma once
#include "Piece.hpp"

class King : public Piece
{
public:
    King(Square pos, const sf::Texture& texture,bool black)
        : Piece(pos, texture,black)
    {
        setSprite(PieceType::King);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};