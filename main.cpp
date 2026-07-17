#include <SFML/Graphics.hpp>
#include "Board.hpp"



int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800, 800}),
        "Chess"
    );


    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
    window.clear(sf::Color::Black);

    // draw things here

    window.display();
    }
}