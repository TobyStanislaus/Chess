#pragma once
#include "Piece.hpp"

class Pawn : public Piece
{
public:
    Pawn(Square pos, const sf::Texture& texture)
        : Piece(pos, texture)
    {
    }

    std::vector<Square> getLegalMoves() override;
    void draw(sf::RenderWindow& window) override;
};