#include "Game.hpp"

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


Square handle_click(sf::RenderWindow& window){
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
    }
    return {-1, -1};
}


void Game::handleInput(sf::RenderWindow& window){  
    Square clicked = handle_click(window);

    if (board.isWaitingToPromote())
    {
        board.handlePromotionClick(clicked);
    }

    for (auto& piece : board.getPieces()){
        Piece* selected = nullptr;
        if (((piece->getPosition() == clicked) && piece->getBlack()==blackTurn)){
            if (piece->isSelected()){selected = piece.get();}
            board.movePiece(board, clicked, blackTurn, selected);
            break;
        } else if (piece->isSelected()){
            bool moved = board.movePiece(board, clicked, blackTurn, piece.get());
            if (moved){blackTurn = !blackTurn;}
            break;
        }
    }

    std::vector<Square> none;
    board.set_potential_moves(none);

    for (auto& piece : board.getPieces())
    {  
        if ((*piece).isSelected()){
            board.set_potential_moves(board.getMoves(*piece, true));
        }
    }
}


void Game::draw(sf::RenderWindow& window){
    display_board(window);
    board.draw(window, blackTurn);
}