#pragma once
#include "Square.hpp"


class Board
{
public:
    Board();
private:
    std::array<std::array<Square, 8>, 8> squares;

};