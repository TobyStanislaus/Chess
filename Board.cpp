#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Pawn.hpp"
#include "Knight.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"

Board::Board()
{
    sf::Texture texture;

    pieces.push_back(std::make_unique<Pawn>(Square{1,0}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,1}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,2}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,3}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,4}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,5}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,6}, texture));
    pieces.push_back(std::make_unique<Pawn>(Square{1,7}, texture));


    pieces.push_back(std::make_unique<Rook>(Square{0,0}, texture));
    pieces.push_back(std::make_unique<Knight>(Square{0,1}, texture));
    pieces.push_back(std::make_unique<Bishop>(Square{0,2}, texture));
    pieces.push_back(std::make_unique<Queen>(Square{0,3}, texture));
    pieces.push_back(std::make_unique<King>(Square{0,4}, texture));
    pieces.push_back(std::make_unique<Bishop>(Square{0,5}, texture));
    pieces.push_back(std::make_unique<Knight>(Square{0,6}, texture));
    pieces.push_back(std::make_unique<Rook>(Square{0,7}, texture));
}

void Board::draw(sf::RenderWindow& window){
    for (auto&piece : pieces){
        piece->draw(window);
    }

    for (auto& move : potential_moves){

        sf::CircleShape pawnShape(30.f);
        pawnShape.setOrigin({30.f, 30.f});

        pawnShape.setPosition({
            move.col * 100.f + 50.f,
            move.row * 100.f + 50.f
        });

        pawnShape.setFillColor(sf::Color::Blue);
        window.draw(pawnShape);
    }
    
}

std::vector<std::unique_ptr<Piece>>& Board::getPieces()
{
    return pieces;
}


void Board::set_potential_moves(const std::vector<Square>& new_moves)
{
    potential_moves = new_moves;
};