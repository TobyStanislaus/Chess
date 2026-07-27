#pragma once
#include "Board.hpp"
enum class PlayerType { Human, RandomBot };

class Game
{
private:
    bool blackTurn = false;
    Piece* selectedPiece = nullptr;

    PlayerType whitePlayer = PlayerType::Human;
    PlayerType blackPlayer = PlayerType::RandomBot;
public:
    Board board;
    void handleRandomBotTurn();
    void handleHumanTurn(Square clicked);
    Square handle_click(sf::RenderWindow& window);
    bool handleInput(sf::RenderWindow& window, Square clicked, sf::Clock& clock);
    void draw(sf::RenderWindow& window);
};