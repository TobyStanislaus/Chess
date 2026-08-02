#pragma once
#include "Board.hpp"
enum class PlayerType { Human, RandomBot, MinimaxBot, MCTS };

class Game
{
private:
    bool blackTurn = false;
    Piece* selectedPiece = nullptr;

    PlayerType whitePlayer = PlayerType::Human;
    PlayerType blackPlayer = PlayerType::MCTS;
public:
    Board board;
    void handleRandomBotTurn();
    void handleMinimaxBotTurn();
    void handleMCTSBotTurn();
    void handleHumanTurn(Square clicked);
    Square handle_click(sf::RenderWindow& window);
    bool handleInput(sf::RenderWindow& window, Square clicked, sf::Clock& clock);
    void draw(sf::RenderWindow& window);

    
};