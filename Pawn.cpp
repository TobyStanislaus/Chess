#include "Pawn.hpp"



std::vector<Square> Pawn::getDirections()
{
    if (getBlack())
    {
    return {{position.row + 1, position.col}};
    }  else{
        return {{position.row - 1, position.col}};
    }

}


PieceType Pawn::getType() const
{
    return PieceType::Pawn;
}