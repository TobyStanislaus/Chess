#include "Pawn.hpp"



std::vector<Square> Pawn::getDirections()
{
    return {
        {position.row + 1, position.col}
    };
}