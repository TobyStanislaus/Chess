#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Pawn.hpp"



Board::Board()
{
    sf::Texture texture;

    pieces.push_back(
        std::make_unique<Pawn>(Square{1,0}, texture)
    );

    pieces.push_back(
        std::make_unique<Pawn>(Square{1,1}, texture)
    );
}

void Board::draw(sf::RenderWindow& window){
    for (auto&piece : pieces){
        piece->draw(window);
    }
    
}

std::vector<std::unique_ptr<Piece>>& Board::getPieces()
{
    return pieces;
}