#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.hpp"
#include "Pawn.hpp"
#include "Piece.hpp"


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


bool isLegalMove(Square& clicked, std::vector<Square> moves)
{
    for (auto move : moves)
    {
        if (move == clicked)
            return true;
    }

    return false;
}


void movePiece(Board& board, Square& clicked, Piece& piece){
    if ((clicked.row == piece.getPosition().row)&&(clicked.col == piece.getPosition().col)){
        if (!piece.isSelected()){
            piece.select();
            return;} 
    }

    else if (piece.isSelected()&&(isLegalMove(clicked, board.getMoves(piece)))){
        piece.setPosition(clicked);
    }
    piece.deselect();
}


int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800, 800}),
        "Chess"
    );

    Board board;
    sf::Texture texture;
    Square clicked;

    while (window.isOpen())
    {

        clicked = handle_click(window);

        Piece* selected = nullptr;

        for (auto& piece : board.getPieces()){
            if (piece->isSelected())
            {
                selected = piece.get();
                break;
            }
        }

        if (selected && isLegalMove(clicked, board.getMoves(*selected)))
        {
            Piece* otherPiece = board.getPieceAt(clicked);

            if (otherPiece)
            {
                board.removePieceAt(clicked);
            }

            selected->setPosition(clicked);
            selected->deselect();
        }
        else if (selected && selected->getPosition() == clicked){selected->deselect();}
        else{
            for (auto& piece : board.getPieces())
                {
                    if (piece->getPosition() == clicked)
                    {
                        for (auto& p : board.getPieces())
                        {
                            p->deselect();
                        }

                        piece->select();
                        break;
                    }
                }
            }

        std::vector<Square> none;
        board.set_potential_moves(none);

        for (auto& piece : board.getPieces())
        {  
            if ((*piece).isSelected()){
                board.set_potential_moves(board.getMoves(*piece));
            }
        }

        window.clear(sf::Color::Black);
        
        display_board(window);
        board.draw(window);
        // draw things here

        window.display();
    }
}