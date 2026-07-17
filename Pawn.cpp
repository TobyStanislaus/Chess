#include "Pawn.hpp"



std::vector<Square> Pawn::getLegalMoves()
{
    return {
        {position.row + 1, position.col}
    };
}