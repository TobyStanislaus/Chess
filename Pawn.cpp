#include "Pawn.hpp"


void Pawn::draw(sf::RenderWindow& window){
    sf::CircleShape pawnShape(30.f);
    pawnShape.setOrigin({30.f, 30.f});

    pawnShape.setPosition({
        position.col * 100.f + 50.f,
        position.row * 100.f + 50.f
    });

    if (selected)
        pawnShape.setFillColor(sf::Color::Yellow);
    else
        pawnShape.setFillColor(sf::Color::Green);

    window.draw(pawnShape);
}


std::vector<Square> Pawn::getLegalMoves()
{
    return {
        {position.row + 1, position.col}
    };
}