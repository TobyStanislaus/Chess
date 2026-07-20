#include "Piece.hpp"
#include <iostream>

void Piece::draw(sf::RenderWindow& window){

    sprite.setPosition({
        position.col * 100.f,
        position.row * 100.f
    });

    window.draw(sprite);
}


void Piece::setSprite(PieceType type)
{
    int size = 150;

    int column = 0;
    int row = black ? 0 : 1;

    switch(type)
    {
        case PieceType::Bishop:
            column = 0;
            break;

        case PieceType::King:
            column = 1;
            break;

        case PieceType::Knight:
            column = 2;
            break;

        case PieceType::Pawn:
            column = 3;
            break;

        case PieceType::Queen:
            column = 4;
            break;

        case PieceType::Rook:
            column = 5;
            break;
    }

    sprite.setTextureRect(
        sf::IntRect(
            {column * size, row * size},
            {size, size}
        )
    );

    sprite.setScale({
    100.f / 150.f,
    100.f / 150.f
    });
}