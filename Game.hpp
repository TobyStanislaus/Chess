#pragma once
#include "Board.hpp"

class Game
{
private:
    Board board;
    bool blackTurn = false;
    int turn = 0;
    Piece* selectedPiece = nullptr;
    
public:
    Square handle_click(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window, Square clicked);
    void draw(sf::RenderWindow& window);
};