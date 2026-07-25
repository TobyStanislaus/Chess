#pragma once
#include <SFML/Graphics.hpp>
#include "Square.hpp"


enum class PieceType
{
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

struct Move
{
    Square from;
    Square to;

    PieceType promotionPiece;

    Piece* capturedPiece;

    bool isCastle;
    bool isEnPassant;

};

class Piece
{
public:

    Piece(Square pos, const sf::Texture& texture, bool black)
        : position(pos), sprite(texture), color(sf::Color::Green), black(black), firstMove(true)
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

    virtual std::vector<Square> getDirections() = 0;

    void setFirstMove(){
        if (firstMove){
            firstMove = false;
        }
    }
    
    bool getFirstMove(){
        return firstMove;
    }
       
    Square getPosition() const
    {
        return position;
    }

    void setPosition(Square pos)
    {
    position = pos;
    }

    bool getBlack() const
    {
        return black;
    }

    void draw(sf::RenderWindow& window);

    virtual PieceType getType() const = 0;

    void setSprite(PieceType type);

    bool canEnPassant= false;
protected:
    sf::Sprite sprite;
    Square position;
    bool selected=false;
    sf::Color color;
    bool black;
    bool firstMove;
};
