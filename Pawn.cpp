#include "Pawn.hpp"



std::vector<Square> Pawn::getDirections()
{
    return {
        {position.row + 1, position.col}
    };
}


PieceType Pawn::getType() const
{
    return PieceType::Pawn;
}