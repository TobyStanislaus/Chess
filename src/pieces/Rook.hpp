#pragma once
#include "Piece.hpp"

class Rook : public Piece
{
public:
    Rook(Square pos, const sf::Texture& texture, bool black, bool firstMove)
        : Piece(pos, texture,black,firstMove)
    {
        setSprite(PieceType::Rook);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};