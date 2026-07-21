#pragma once
#include "Piece.hpp"

class Queen : public Piece
{
public:
    Queen(Square pos, const sf::Texture& texture,bool black)
        : Piece(pos, texture, black)
    {
        setSprite(PieceType::Queen);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};