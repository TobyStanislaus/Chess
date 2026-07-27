#pragma once
#include "Piece.hpp"

class Knight : public Piece
{
public:
    Knight(Square pos, const sf::Texture& texture,bool black, bool firstMove)
        : Piece(pos, texture,black, firstMove)
    {
        setSprite(PieceType::Knight);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};