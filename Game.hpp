#pragma once
#include "Board.hpp"
enum class PlayerType { Human, RandomBot };

class Game
{
private:
    Board board;
    bool blackTurn = false;
    int turn = 1;
    Piece* selectedPiece = nullptr;

    PlayerType whitePlayer = PlayerType::RandomBot;
    PlayerType blackPlayer = PlayerType::RandomBot;
public:
    Square handle_click(sf::RenderWindow& window);
    bool handleInput(sf::RenderWindow& window, Square clicked);
    void draw(sf::RenderWindow& window);
};