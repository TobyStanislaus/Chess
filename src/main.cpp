#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "Game.hpp"


int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1000, 800}),
        "Chess"
    );


    Game game;
    sf::Clock clock;

    while (window.isOpen())
    {
        Square clicked = game.handle_click(window);

        if (!game.board.getGameOver())
        {
            game.handleInput(clicked, clock);
        }

        window.clear(sf::Color::Black);
        game.draw(window);
        window.display();
    }
}