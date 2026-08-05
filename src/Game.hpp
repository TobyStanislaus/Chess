#pragma once
#include "Board.hpp"
enum class PlayerType { Human, RandomBot, MinimaxBot, MCTS };

class Game
{
private:
    bool paused = false;
    bool blackTurn = false;
    Piece* selectedPiece = nullptr;

    PlayerType whitePlayer = PlayerType::Human;
    PlayerType blackPlayer = PlayerType::MinimaxBot;
public:
    Board board;
    void display_board(sf::RenderWindow& window);
    void handleRandomBotTurn();
    void handleMinimaxBotTurn();
    void handleMCTSBotTurn();
    void handleHumanTurn(Square clicked);
    Square handle_click(sf::RenderWindow& window);
    bool handleInput(Square clicked, sf::Clock& clock);
    void draw(sf::RenderWindow& window);
    void useButton(Square clicked);
    float getPlayerTypeY(PlayerType player);
    
};