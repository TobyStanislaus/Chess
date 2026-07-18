#include "King.hpp"



std::vector<Square> King::getDirections()
{
    return {
        {position.row + 1, position.col-1},
        {position.row + 1, position.col},
        {position.row + 1, position.col+1},

        {position.row, position.col-1},
        {position.row, position.col+1},

        {position.row-1, position.col-1},
        {position.row-1, position.col},
        {position.row-1, position.col+1},
    };
}