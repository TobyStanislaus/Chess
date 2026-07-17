#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Pawn.hpp"

void display(sf::RenderWindow& window){
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


int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800, 800}),
        "Chess"
    );
    Board board;
    sf::Texture texture;

    Pawn pawn({0, 1}, texture);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

    window.clear(sf::Color::Black);
    
    display(window);
    pawn.draw(window);

    // draw things here

    window.display();
    }
}