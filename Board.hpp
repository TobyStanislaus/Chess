#pragma once

#include <vector>
#include <memory>
#include "Piece.hpp"

class Board
{
public:
    Board();

    void draw(sf::RenderWindow& window);
    std::vector<std::unique_ptr<Piece>>& getPieces();
    
private:
    std::vector<std::unique_ptr<Piece>> pieces;
};