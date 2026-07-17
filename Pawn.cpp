#include "Pawn.hpp"


void Pawn::draw(sf::RenderWindow& window){
    sf::CircleShape pawnShape(30.f);
    pawnShape.setOrigin({30.f, 30.f});

    pawnShape.setPosition({
        position.col * 100.f + 50.f,
        position.row * 100.f + 50.f
    });

    window.draw(pawnShape);
}
