#pragma once
#include "Board.hpp"

class Game
{
private:
    Board board;
    bool blackTurn = false;
    Piece* selectedPiece = nullptr;
    
public:
    void handleInput(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};