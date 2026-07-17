#pragma once
#include <SFML/Graphics.hpp>
#include "Square.hpp"

class Piece
{
public:

    Piece(Square pos, const sf::Texture& texture)
        : position(pos), sprite(texture)
    {
    }
    
    virtual ~Piece() = default;

    virtual void draw(sf::RenderWindow&) = 0;

    //virtual std::vector<Square> getLegalMoves() = 0;


    Square getPosition() const
    {
        return position;
    }
 
protected:
    sf::Sprite sprite;
    Square position;
};