#pragma once
#include <SFML/Graphics.hpp>
#include "Square.hpp"

class Piece
{
public:

    Piece(Square pos, const sf::Texture& texture)
        : position(pos), sprite(texture), color(sf::Color::Green)
    {
    }
    
    void select()
    {
        selected = true;
    }

    void deselect()
    {
        selected = false;
    }

    bool isSelected() const
    {
        return selected;
    }


    virtual ~Piece() = default;

    //virtual std::vector<Square> getLegalMoves() = 0;

    virtual std::vector<Square> getLegalMoves() = 0;


    Square getPosition() const
    {
        return position;
    }

    void setPosition(Square pos)
    {
    position = pos;
    }

    void draw(sf::RenderWindow& window);
protected:
    sf::Sprite sprite;
    Square position;
    bool selected=false;
    sf::Color color;
};