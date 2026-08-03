#include "Bishop.hpp"

std::vector<Square> Bishop::getDirections(){
    return {{1,1},
            {-1,-1},
            {-1,1},
            {1,-1},
        };

}

PieceType Bishop::getType() const
{
    return PieceType::Bishop;
}