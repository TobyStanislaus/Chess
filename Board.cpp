#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Pawn.hpp"
#include "Knight.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"

Board::Board()
{
    sf::Texture texture;
    bool black = true;
    pieces.push_back(std::make_unique<Pawn>(Square{1,0}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,1}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,2}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,3}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,4}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,5}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,6}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{1,7}, texture, black));

    pieces.push_back(std::make_unique<Rook>(Square{0,0}, texture, black));
    pieces.push_back(std::make_unique<Knight>(Square{0,1}, texture, black));
    pieces.push_back(std::make_unique<Bishop>(Square{0,2}, texture, black));
    pieces.push_back(std::make_unique<King>(Square{0,3}, texture, black));
    pieces.push_back(std::make_unique<Queen>(Square{0,4}, texture, black));
    pieces.push_back(std::make_unique<Bishop>(Square{0,5}, texture, black));
    pieces.push_back(std::make_unique<Knight>(Square{0,6}, texture, black));
    pieces.push_back(std::make_unique<Rook>(Square{0,7}, texture, black));

    black = false;
    pieces.push_back(std::make_unique<Pawn>(Square{6,0}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,1}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,2}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,3}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,4}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,5}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,6}, texture, black));
    pieces.push_back(std::make_unique<Pawn>(Square{6,7}, texture, black));

    pieces.push_back(std::make_unique<Rook>(Square{7,0}, texture, black));
    pieces.push_back(std::make_unique<Knight>(Square{7,1}, texture, black));
    pieces.push_back(std::make_unique<Bishop>(Square{7,2}, texture, black));
    pieces.push_back(std::make_unique<Queen>(Square{7,3}, texture, black));
    pieces.push_back(std::make_unique<King>(Square{7,4}, texture, black));
    pieces.push_back(std::make_unique<Bishop>(Square{7,5}, texture, black));
    pieces.push_back(std::make_unique<Knight>(Square{7,6}, texture, black));
    pieces.push_back(std::make_unique<Rook>(Square{7,7}, texture, black));
}

void Board::draw(sf::RenderWindow& window){
    for (auto&piece : pieces){
        piece->draw(window);
    }

    for (auto& move : potential_moves){

        sf::CircleShape pawnShape(30.f);
        pawnShape.setOrigin({30.f, 30.f});

        pawnShape.setPosition({
            move.col * 100.f + 50.f,
            move.row * 100.f + 50.f
        });

        pawnShape.setFillColor(sf::Color::Blue);
        window.draw(pawnShape);
    }
    
}


std::vector<std::unique_ptr<Piece>>& Board::getPieces()
{
    return pieces;
}


void Board::set_potential_moves(const std::vector<Square>& new_moves)
{
    potential_moves = new_moves;
};

bool Board::inBounds(Square& current){
    return (current.row>=0 && current.col>=0 && current.row<8 && current.col<8);
}


bool Board::friendlyPiece(Square current){
    for (auto& piece : pieces){
        if ((*piece).getPosition() == current){return true;};
    }
    return false;
}


std::vector<Square> Board::getSlidingMoves(Piece& piece)
{
    std::vector<Square> moves;

    for (Square dir : piece.getDirections())
    {
        Square current = piece.getPosition();

        while (true)
        {
            current.row += dir.row;
            current.col += dir.col;

            if (!inBounds(current))
                break;

            if (friendlyPiece(current))
                break;

            moves.push_back(current);

            //if (enemyPiece(current, piece))
            //    break;
        }
    }

    return moves;
}

std::vector<Square> Board::getNormalMoves(Piece& piece)
{
    std::vector<Square> moves;

    for (Square dir : piece.getDirections())
    {
        if (inBounds(dir) && !friendlyPiece(dir))
            moves.push_back(dir);

        //if (enemyPiece(current, piece))
        //    break;
    
    }

    return moves;
}
std::vector<Square> Board::getPawnMoves(Piece& piece)
{
    std::vector<Square> moves;

    for (Square dir : piece.getDirections())
    {
        if (inBounds(dir) && !friendlyPiece(dir))
            moves.push_back(dir);

        //if (enemyPiece(current, piece))
        //    break;
    
    }

    return moves;
}

std::vector<Square> Board::getMoves(Piece& piece)
{
    switch (piece.getType())
    {
    case PieceType::King:
    case PieceType::Knight:
        return getNormalMoves(piece);

    case PieceType::Bishop:
    case PieceType::Rook:
    case PieceType::Queen:
        return getSlidingMoves(piece);
    
    case PieceType::Pawn:
        return getPawnMoves(piece);
    }
    return {};
}
