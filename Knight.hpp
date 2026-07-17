#pragma once
#include "Piece.hpp"

class Knight : public Piece
{
public:
    Knight(Square pos, const sf::Texture& texture)
        : Piece(pos, texture)
    {
        color = sf::Color::Red;
    }

    std::vector<Square> getLegalMoves() override;
};