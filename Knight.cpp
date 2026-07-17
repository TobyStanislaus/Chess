#include "Knight.hpp"



std::vector<Square> Knight::getLegalMoves()
{
    return {
        {position.row + 2, position.col+1},
        {position.row + 2, position.col-1},
        {position.row - 2, position.col+1},
        {position.row - 2, position.col-1},

        {position.row + 1, position.col+2},
        {position.row + 1, position.col-2},
        {position.row - 1, position.col+2},
        {position.row - 1, position.col-2},
    };
}