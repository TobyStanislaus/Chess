#include "Game.hpp"
#include <iostream>

void display_board(sf::RenderWindow& window){
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            sf::RectangleShape square({100.f, 100.f});

            square.setPosition({
                col * 100.f,
                row * 100.f
            });

            if ((row + col) % 2 == 0)
                square.setFillColor(sf::Color::White);
            else
                square.setFillColor(sf::Color::Black);

            window.draw(square);
        }
    }
}


Square Game::handle_click(sf::RenderWindow& window){
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        window.close();

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                sf::Vector2i pixelPos = mouse->position;

                return {
                    pixelPos.y / 100,
                    pixelPos.x / 100
                };
            }
        }
        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Q)
            {
                return {-50,-50};
            }
        }

    }
    return {-1, -1};
}


void Game::handleRandomBotTurn()
{
    Move move = board.makeRandomMove(blackTurn);
    if (move.from.col != -1){
        board.makeMove(move, this->blackTurn); // see note below on member vs param
        this->blackTurn = !this->blackTurn;
    } else {
        board.checkIfILost(blackTurn);
    }
}


void Game::handleHumanTurn(Square clicked)
{
    board.checkIfILost(blackTurn);
    Piece* selected = nullptr;

    for (auto& piece : board.getPieces()){
        if ((piece->getPosition() == clicked) && piece->getBlack() == blackTurn){
            if (piece->isSelected()){ selected = piece.get(); }
            board.movePiece(clicked, blackTurn, selected);
            break;
        } else if (piece->isSelected()){
            bool moved = board.movePiece(clicked, blackTurn, piece.get());
            if (moved) blackTurn = !blackTurn;
            break;
        }
    }

    if (board.isWaitingToPromote())
    {
        if (board.handlePromotionClick(clicked, blackTurn)) blackTurn = !blackTurn;
    }

    std::vector<Move> none;
    board.set_potential_moves(none);

    for (auto& piece : board.getPieces())
    {
        if (piece->isSelected()){
            board.set_potential_moves(board.getMoves(*piece, true));
        }
    }
}


bool Game::handleInput(sf::RenderWindow& window, Square clicked, sf::Clock& clock){  
    
    if (clicked.col == -50 && board.getMoveHistory().size()>0 && !board.isWaitingToPromote()){
        blackTurn = !blackTurn;
        board.undoMove(blackTurn);
        return !board.getGameOver();
    }

    PlayerType currentPlayer = blackTurn ? blackPlayer : whitePlayer;

    if (currentPlayer == PlayerType::RandomBot && !board.isWaitingToPromote() && clock.getElapsedTime().asSeconds()>=1)
    {
        clock.restart();
        handleRandomBotTurn();
    }else if (currentPlayer == PlayerType::Human)
    {
        handleHumanTurn(clicked);
    }

    return !board.getGameOver();
}


void Game::draw(sf::RenderWindow& window){
    display_board(window);
    board.draw(window, blackTurn);
}