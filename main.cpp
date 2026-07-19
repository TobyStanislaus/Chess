#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.hpp"
#include "Game.hpp"





int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800, 800}),
        "Chess"
    );


    Game game;

    while (window.isOpen())
    {
        game.handleInput(window);

        window.clear(sf::Color::Black);
        
        game.draw(window);

        window.display();
    }
}