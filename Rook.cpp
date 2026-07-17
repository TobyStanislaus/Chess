#include "Rook.hpp"



std::vector<Square> Rook::getLegalMoves()
{
    std::vector<Square> legalMoves;
    for (int i=0; i<8;i++){
        if (i!=position.col){legalMoves.push_back(Square(position.row, i));}
        if (i!=position.row){legalMoves.push_back(Square(i, position.col));}
    }
    
    return legalMoves;
}