#pragma once
#include "Piece.hpp"

class Pawn : public Piece
{
public:
    Pawn(Square pos, const sf::Texture& texture)
        : Piece(pos, texture)
    {
    }

    void draw(sf::RenderWindow& window) override;
    
};