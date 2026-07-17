#include <SFML/Graphics.hpp>
#include "Board.hpp"

Board::Board(){};

void Board::display(sf::RenderWindow& window){
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