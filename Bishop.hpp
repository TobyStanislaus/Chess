#pragma once
#include "Piece.hpp"

class Bishop : public Piece
{
public:
    Bishop(Square pos, const sf::Texture& texture,bool black, bool firstMove)
        : Piece(pos, texture,black, firstMove)
    {
        setSprite(PieceType::Bishop);
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;
    
};