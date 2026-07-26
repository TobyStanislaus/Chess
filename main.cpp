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
    bool keepPlaying = true;
    while (window.isOpen() && keepPlaying)
    {
        Square clicked = game.handle_click(window);
        keepPlaying = game.handleInput(window, clicked);

        window.clear(sf::Color::Black);
        
        game.draw(window);

        window.display();
    }
}