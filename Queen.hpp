#pragma once
#include "Piece.hpp"

class Queen : public Piece
{
public:
    Queen(Square pos, const sf::Texture& texture,bool black, bool firstMove)
        : Piece(pos, texture, black, firstMove)
    {
        setSprite(PieceType::Queen);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
};