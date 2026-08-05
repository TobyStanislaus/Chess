#include "Game.hpp"


void Game::display_board(sf::RenderWindow& window){
    //Chess Board
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


    sf::RectangleShape sidePanel({200.f, 600.f});
    sidePanel.setPosition({800.f, 200.f});
    sidePanel.setFillColor(sf::Color(220, 220, 220));
    window.draw(sidePanel);

    sf::RectangleShape playerTypeBox({200.f, 100.f});
    playerTypeBox.setPosition({800.f, getPlayerTypeY(blackTurn ? blackPlayer : whitePlayer)});
    playerTypeBox.setFillColor(sf::Color(100, 200, 100));
    window.draw(playerTypeBox);
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
        board.makeMove(move, blackTurn); 
        blackTurn = !blackTurn;
    } else {
        board.checkIfILost(blackTurn);
    }
}


void Game::handleMinimaxBotTurn(){
    Move move = board.findBestMove(blackTurn, 3); 
    if (move.from.col != -1){
        board.makeMove(move, blackTurn);
        blackTurn = !blackTurn;
    } else {
        board.checkIfILost(blackTurn);
    }
}


void Game::handleMCTSBotTurn(){
    Move move = board.findBestMoveMCTS(blackTurn, 200);
    if (move.from.col != -1){
        board.makeMove(move, blackTurn);
        blackTurn = !blackTurn;
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


bool Game::handleInput(Square clicked, sf::Clock& clock){  
    useButton(clicked);

    if (clicked.col == -50 && board.getMoveHistory().size()>0 && !board.isWaitingToPromote()){
        clock.restart();
        blackTurn = !blackTurn;
        board.undoMove(blackTurn);
        return !board.getGameOver();
    }

    if (paused) {return true;}

    PlayerType currentPlayer = blackTurn ? blackPlayer : whitePlayer;


    if (currentPlayer == PlayerType::Human)
    {
        handleHumanTurn(clicked);
        clock.restart();
    } else if (!board.isWaitingToPromote() && clock.getElapsedTime().asMilliseconds()>=500){
        clock.restart();
        if (currentPlayer == PlayerType::RandomBot)
        {
            handleRandomBotTurn();
        }else if (currentPlayer == PlayerType::MinimaxBot)
        {
            handleMinimaxBotTurn();
        }else if (currentPlayer == PlayerType::MCTS){
            handleMCTSBotTurn();
        }

    }


    return !board.getGameOver();
}


void Game::draw(sf::RenderWindow& window){
    display_board(window);
    board.draw(window, blackTurn, paused);
}

void Game::useButton(Square clicked){
    PlayerType* playerToChange = blackTurn ? &blackPlayer : &whitePlayer;
    if (clicked.col<=7){return;}
    if (clicked.row == 7){*playerToChange = PlayerType::RandomBot;}
    if (clicked.row == 6){*playerToChange = PlayerType::MCTS;}
    if (clicked.row == 5){*playerToChange = PlayerType::MinimaxBot;}
    if (clicked.row == 4){*playerToChange = PlayerType::Human;}
    if (clicked.row == 2){
        paused = !paused;
    }
}

float Game::getPlayerTypeY(PlayerType player)
{
    switch (player)
    {
        case PlayerType::RandomBot: return 700.f;
        case PlayerType::MCTS: return 600.f;
        case PlayerType::MinimaxBot: return 500.f;
        case PlayerType::Human: return 400.f;
    }

    return 700.f; // fallback
}