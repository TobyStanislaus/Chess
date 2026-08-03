#pragma once
#include "Piece.hpp"

class Pawn : public Piece
{
public:
    Pawn(Square pos, const sf::Texture& texture,bool black, bool firstMove)
        : Piece(pos, texture, black, firstMove)
    {
        setSprite(PieceType::Pawn);
        //color = sf::Color::Green;
    }

    std::vector<Square> getDirections() override;
    PieceType getType() const override;

    bool canEnPassant=false;

};