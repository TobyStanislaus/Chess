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

    void setFirstMove(bool value){
        firstMove = value;

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


struct Move
{
    Square from;
    Square to;

    PieceType promotionPiece = PieceType::Queen;
    bool isPromotion = false;

    Piece* capturedPiece = nullptr;

    bool isCastle = false;
    bool isEnPassant = false;

    PieceType movedPiece = PieceType::Pawn;
    PieceType capturedPieceType = PieceType::Pawn;
    bool capturedPieceBlack = false;

    bool movedPieceFirstMoveBefore = true;   // was selected->getFirstMove() before this move
    bool hadEnPassantTarget = false;         // was any piece eligible before this move?
    Square enPassantTargetSquare = {-1,-1};
};