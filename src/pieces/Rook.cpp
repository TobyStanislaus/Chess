#include "Rook.hpp"

std::vector<Square> Rook::getDirections(){
    return {
        {1,0},
        {0,1},
        {-1,0},
        {0,-1},
    };
}

PieceType Rook::getType() const
{
    return PieceType::Rook;
}