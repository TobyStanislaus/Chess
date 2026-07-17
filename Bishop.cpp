#include "Bishop.hpp"



std::vector<Square> Bishop::getLegalMoves()
{
    std::vector<Square> legalMoves;

    // top-right
    for (int i = 1; position.row - i >= 0 && position.col + i < 8; i++)
    {
        legalMoves.push_back(Square{position.row - i, position.col + i});
    }

    // top-left
    for (int i = 1; position.row - i >= 0 && position.col - i >= 0; i++)
    {
        legalMoves.push_back(Square{position.row - i, position.col - i});
    }

    // bottom-right
    for (int i = 1; position.row + i < 8 && position.col + i < 8; i++)
    {
        legalMoves.push_back(Square{position.row + i, position.col + i});
    }

    // bottom-left
    for (int i = 1; position.row + i < 8 && position.col - i >= 0; i++)
    {
        legalMoves.push_back(Square{position.row + i, position.col - i});
    }

    return legalMoves;
}