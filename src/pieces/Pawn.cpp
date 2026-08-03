#include "Pawn.hpp"




std::vector<Square> Pawn::getDirections()
{
    int direction;
    if (getBlack()){direction = 1;}  
    else{direction = -1;}   

    if (firstMove){
        return {{position.row + direction, position.col},
                {position.row + 2*direction, position.col},
                };}

    return {{position.row + direction, position.col}};
}

PieceType Pawn::getType() const 
{
    return PieceType::Pawn;
}