#include "Queen.hpp"
std::vector<Square> Queen::getDirections(){

    return{
        {1,-1},
        {1,0},
        {1,1},

        {0,-1},
        {0,1},

        {-1,-1},
        {-1,0},
        {-1,1},
    };
}

PieceType Queen::getType() const
{
    return PieceType::Queen;
}