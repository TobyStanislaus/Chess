#pragma once
struct Square
{
    int row;
    int col;

    bool operator==(const Square& other) const
    {
        return row == other.row && col == other.col;
    }
};