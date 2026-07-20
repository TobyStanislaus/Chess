#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Pawn.hpp"
#include "Knight.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include <iostream>
Board::Board()
{
    if (!texture.loadFromFile("assets/Chess.png"))
    {
        std::cout << "Texture failed\n";
    }

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

bool Board::inBounds(Square& newPos){
    return (newPos.row>=0 && newPos.col>=0 && newPos.row<8 && newPos.col<8);
}


Piece* Board::getPieceAt(Square newPos)
{
    for (auto& piece : pieces)
    {
        if (piece->getPosition() == newPos)
            return piece.get();
    }

    return nullptr;
}


std::vector<Square> Board::getSlidingMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Square> moves;

    for (Square dir : piece.getDirections())
    {
        Square newPos = piece.getPosition();

        while (true)
        {
            newPos.row += dir.row;
            newPos.col += dir.col;

            if (!inBounds(newPos))
                break;

            Piece* otherPiece = getPieceAt(newPos);

            if (otherPiece == nullptr){ 
                if (careAboutCheck){
                    std::cout<< testMoveForCheck(piece, newPos)<<std::endl;
                }          
                moves.push_back(newPos);
            } else if (otherPiece->getBlack() == piece.getBlack())
                break;
            else{
                moves.push_back(newPos);
                break;
            }
        }
    }

    return moves;
}


std::vector<Square> Board::getNormalMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Square> moves;

    for (Square newPos : piece.getDirections())
    {   
        Piece* otherPiece = getPieceAt(newPos);

        if (!inBounds(newPos)){continue;}
        else if (otherPiece == nullptr || !(otherPiece->getBlack() == piece.getBlack()))
        {
            if (otherPiece == nullptr && careAboutCheck){
                std::cout<< testMoveForCheck(piece, newPos)<<std::endl;
            } 
            moves.push_back(newPos);
        }
        
    }

    return moves;
}


std::vector<Square> Board::getPawnMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Square> moves;

    for (Square newPos : piece.getDirections())
    {
        Piece* otherPiece = getPieceAt(newPos);

        if (!inBounds(newPos)){continue;}
        if (otherPiece == nullptr || !(otherPiece->getBlack() == piece.getBlack()))
        {
            if (otherPiece == nullptr && careAboutCheck){
                std::cout<< "Hi "<<testMoveForCheck(piece, newPos)<<std::endl;
            } 
            moves.push_back(newPos);
        }
    }

    return moves;
}


std::vector<Square> Board::getMoves(Piece& piece, bool careAboutCheck)
{
    switch (piece.getType())
    {
    case PieceType::King:
    case PieceType::Knight:
        return getNormalMoves(piece,careAboutCheck);

    case PieceType::Bishop:
    case PieceType::Rook:
    case PieceType::Queen:
        return getSlidingMoves(piece,careAboutCheck);
    
    case PieceType::Pawn:
        return getPawnMoves(piece,careAboutCheck);
    }
    return {};
}


void Board::removePieceAt(Square square)
{
    pieces.erase(
        std::remove_if(
            pieces.begin(),
            pieces.end(),
            [&](const std::unique_ptr<Piece>& piece)
            {
                return piece->getPosition() == square;
            }),
        pieces.end());
}


bool Board::isLegalMove(Square& clicked, std::vector<Square> moves)
{
    for (auto move : moves)
    {
        if (move == clicked)
            return true;
    }

    return false;
}


bool Board::movePiece(Board& board, Square& clicked, bool blackTurn)
{
    Piece* selected = nullptr;

    for (auto& piece : board.getPieces())
    {
        if (piece->isSelected())
        {
            selected = piece.get();
            break;
        }
    }

    // Moving selected piece
    if (selected && isLegalMove(clicked, getMoves(*selected, true)))
    {
        Piece* otherPiece = board.getPieceAt(clicked);

        if (otherPiece)
        {
            board.removePieceAt(clicked);
        }

        selected->setPosition(clicked);
        selected->deselect();

        return true; // <-- actual move happened
    }

    // Clicking selected piece again
    else if (selected && selected->getPosition() == clicked)
    {
        selected->deselect();
        return false;
    }

    // Selecting a piece
    else
    {
        for (auto& piece : board.getPieces())
        {
            if ((piece->getPosition() == clicked) && piece->getBlack() == blackTurn)
            {
                for (auto& p : board.getPieces())
                {
                    p->deselect();
                }

                piece->select();

                return false; // <-- only selected, no move
            }
        }
    }

    return false;
}


bool Board::testMoveForCheck(Piece& piece, Square& newPos){
    Square originalPosition = piece.getPosition();

    piece.setPosition(newPos);
    bool result = checkForCheck(piece.getBlack());

    piece.setPosition(originalPosition);
    return (result);
} 


bool Board::checkForCheck(bool isBlack)
{
    Square kingPosition;

    // Find the king
    for (auto& piece : pieces)
    {
        if (piece->getType()==PieceType::King && piece->getBlack() == isBlack)
        {
            kingPosition = piece->getPosition();
            break;
        }
    }

    // Check if any enemy piece attacks it
    for (auto& enemyPiece : pieces)
    {
        if (enemyPiece->getBlack() != isBlack)
        {
            std::vector<Square> moves = getMoves(*enemyPiece, false);

            for (Square move : moves)
            {
                if (move == kingPosition)
                    return true;
            }
        }
    }
    return false;
}