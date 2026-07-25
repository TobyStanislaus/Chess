#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Pawn.hpp"
#include "Knight.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

Board::Board()
{
    if (!texture.loadFromFile("assets/Chess.png"))
    {
        std::cout << "Texture failed\n";
    }

    if (!font.openFromFile("assets/arial.TTF"))
    {
        std::cout << "Font failed\n";
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

    pieces.push_back(std::make_unique<Queen>(Square{0,3}, texture, black));
    pieces.push_back(std::make_unique<King>(Square{0,4}, texture, black));

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


void Board::draw(sf::RenderWindow& window, bool blackTurn){
    for (auto&piece : pieces){
        piece->draw(window);
    }

    for (auto& move : potential_moves){

        sf::CircleShape pawnShape(30.f);
        pawnShape.setOrigin({30.f, 30.f});

        pawnShape.setPosition({
            move.to.col * 100.f + 50.f,
            move.to.row * 100.f + 50.f
        });

        pawnShape.setFillColor(sf::Color::Blue);
        window.draw(pawnShape);
    }
    
    if (gameOver)
    {
        // Dark translucent background
        sf::RectangleShape overlay({800.f, 800.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));

        window.draw(overlay);

        // White box
        sf::RectangleShape box({500.f, 150.f});
        box.setOrigin({250.f, 75.f});
        box.setPosition({400.f, 400.f});
        box.setFillColor(sf::Color(240, 240, 240, 220));

        window.draw(box);

        // Text
        sf::Text text(font, gameOverMessage, 40);
        text.setFillColor(sf::Color::Black);

        auto bounds = text.getLocalBounds();
        text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        text.setPosition({400.f, 400.f});

        window.draw(text);
    }

    else if (waitingForPromotion)
    {
        // Dim the board
        sf::RectangleShape dim({800.f, 800.f});
        dim.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(dim);


        // Popup box
        sf::RectangleShape box({600.f, 200.f});
        box.setPosition({100.f, 300.f});
        box.setFillColor(sf::Color(230, 230, 230));

        window.draw(box);


        // Title
        sf::Text title(font, "Promotion", 100);
        title.setFillColor(sf::Color::Black);
        title.setPosition({150.f, 300.f});

        window.draw(title);

        // Draw the pieces
        Queen queen({4, 2}, texture, !blackTurn);
        Rook rook({4, 3}, texture, !blackTurn);
        Bishop bishop({4, 4}, texture, !blackTurn);
        Knight knight({4, 5}, texture, !blackTurn);

        // Better: make a draw version that accepts position
        queen.draw(window);
        rook.draw(window);
        bishop.draw(window);
        knight.draw(window);
    }
}


std::vector<std::unique_ptr<Piece>>& Board::getPieces()
{
    return pieces;
}


void Board::set_potential_moves(const std::vector<Move>& new_moves)
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


void Board::decideWhetherToAddMove(Move& newMove, std::vector<Move>& moves, bool careAboutCheck){
    if (careAboutCheck){
        if (!testMoveForCheck(newMove)){
            moves.push_back(newMove);
        }
    }else{
        moves.push_back(newMove);
    }
}


std::vector<Move> Board::getSlidingMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Move> moves;

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
            Move newMove = {piece.getPosition(), newPos};

            if (otherPiece == nullptr){ //No piece
                decideWhetherToAddMove(newMove, moves, careAboutCheck);
                
            } else if (otherPiece->getBlack() == piece.getBlack()) // piece on same team
                break;
            else{ // piece on other team
                decideWhetherToAddMove(newMove, moves, careAboutCheck);
                break;
            }
        }
    }

    return moves;
}


void Board::handleCastling(Piece& piece, std::vector<Move>& moves){
    bool black = piece.getBlack();
    int success = canCastle(black);
    if (success==1){moves.push_back(Move{piece.getPosition(),Square{piece.getPosition().row, 2}});}
    else if (success==2){Move{piece.getPosition(),Square{piece.getPosition().row, 6}};}

    else if (success==3){Move{piece.getPosition(),Square{piece.getPosition().row, 2}};
                            Move{piece.getPosition(),Square{piece.getPosition().row, 6}};}
}


std::vector<Move> Board::getNormalMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Move> moves;

    if (careAboutCheck && piece.getType() == PieceType::King){
        handleCastling(piece, moves);
    }

    for (Square newPos : piece.getDirections())
    {
        Piece* otherPiece = getPieceAt(newPos);

        if (!inBounds(newPos)){continue;}
        else if (otherPiece == nullptr || otherPiece->getBlack() != piece.getBlack())
        { 
            Move newMove = {piece.getPosition(),newPos};
            if (otherPiece == nullptr && careAboutCheck){ // nothing there and care about check
                if (!testMoveForCheck(newMove)){
                    moves.push_back(newMove);
                }
            } else { // different team or do not care about check
                decideWhetherToAddMove(newMove, moves, careAboutCheck);
            }
        }
    }

    return moves;
}


void Board::checkDiag(Square& currPos, Piece& piece, bool& careAboutCheck, std::vector<Move>& moves){
    Piece* diagPiece = getPieceAt(currPos);
    Move newMove = {piece.getPosition(), currPos};
    
    if (diagPiece != nullptr &&
        diagPiece->getBlack() != piece.getBlack())
    {
        if (careAboutCheck){
            if (!testMoveForCheck(newMove)){
                moves.push_back(newMove);
            }
        } else{
            decideWhetherToAddMove(newMove, moves, careAboutCheck);
        }
    }
}


std::vector<Piece*> Board::enPassant(Piece& piece, std::vector<Move>& moves, int& direction){
    int row = piece.getPosition().row;
    int col = piece.getPosition().col;
    Piece* leftPiece = getPieceAt(Square{row, col-1});
    Piece* rightPiece = getPieceAt(Square{row, col+1});
           

    if ((piece.getBlack()&&row==4)||(!piece.getBlack()&&row==3)){
        if (leftPiece && leftPiece->canEnPassant){
            moves.push_back(Move{piece.getPosition(),{row+direction,col-1}});
        }
        if (rightPiece && rightPiece->canEnPassant){
            moves.push_back(Move{piece.getPosition(),Square{row+direction,col+1}});
        }
    }
    return {leftPiece, rightPiece};
}


std::vector<Move> Board::getPawnMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Move> moves;

    int direction;
    if (piece.getBlack()){direction = 1;}  
    else{direction = -1;}  

    Square currPos = piece.getPosition();
    currPos.row+=direction;currPos.col+=direction;
    checkDiag(currPos, piece, careAboutCheck, moves);
    currPos.col-=direction*2;
    checkDiag(currPos, piece, careAboutCheck, moves);
    enPassant(piece, moves, direction);

    for (Square newPos : piece.getDirections())
    {
        Piece* forwardPiece = getPieceAt(newPos);
        Move newMove = {piece.getPosition(), newPos};

        if (!inBounds(newPos)){continue;}
        if (forwardPiece == nullptr)
        {
            if (careAboutCheck){
                if (!testMoveForCheck(newMove)){
                    moves.push_back(newMove);
                }
            } else{
                decideWhetherToAddMove(newMove, moves, careAboutCheck);
                
            }
        }

        if (forwardPiece){break;}
    }
    return moves;
}


std::vector<Move> Board::getMoves(Piece& piece, bool careAboutCheck)
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


std::vector<Move> Board::getAllLegalMoves(bool black)
{
    std::vector<Move> moves;

    for(auto& piece : pieces)
    {
        if(piece->getBlack() == black)
        {
            auto newMoves = getMoves(*piece,true);

            for(auto move : newMoves)
            {
                moves.push_back(move);
            }
        }
    }

    return moves;
}


Move Board::makeRandomMove(bool black){
    std::vector<Move> moves = getAllLegalMoves(black);

    if (moves.empty())
    {
    return {{-1,-1}, {-1,-1}};
    }

    std::uniform_int_distribution<int> randI(0, moves.size()-1);
    return moves[randI(gen)];
}


std::unique_ptr<Piece> Board::removePieceAt(Square square)
{
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        if ((*it)->getPosition() == square)
        {
            std::unique_ptr<Piece> removed = std::move(*it);
            pieces.erase(it);
            return removed;
        }
    }

    return nullptr;
}

       
bool Board::isLegalMove(Square& clicked, std::vector<Move> moves)
{
    for (auto move : moves)
    {
        if (move.to == clicked)
            return true;
    }

    return false;
}


void Board::makeMove(Move& move, bool& blackTurn){
    Square clicked = move.to;
    Piece* selected = getPieceAt(move.from);


    Piece* otherPiece = getPieceAt(clicked);
    int differenceRow = clicked.row - selected->getPosition().row;
    int differenceCol = clicked.col - selected->getPosition().col;
    
    if (otherPiece)
    {
        removePieceAt(clicked);
    } else{
        if (clicked.row != selected->getPosition().row && 
            clicked.col != selected->getPosition().col &&
            ((*selected).getType() == PieceType::Pawn)){
                removePieceAt(Square{selected->getPosition().row, 
                                    clicked.col});
        }
    }

    selected->setPosition(clicked);
    selected->deselect();

    for (auto& piece : getPieces())
    {
        piece->canEnPassant = false;
    }

    if (((*selected).getType() == PieceType::Pawn)){
        
        if (differenceRow == 2 || differenceRow == -2){selected->canEnPassant=true;}
        if (selected != nullptr){detectIfPromotion(*selected);}
    }

    if ((*selected).getType() == PieceType::King){
        if (differenceCol == 2 || differenceCol == -2){
            int row;
            if (blackTurn){row=0;}
            else {row=7;}
            if (clicked.col == 2){
                removePieceAt(Square{row,0});
                addPiece(PieceType::Rook, {row,3}, blackTurn);
            }else if (clicked.col == 6){
                removePieceAt(Square{row,7});
                addPiece(PieceType::Rook, {row,5}, blackTurn);
            }
        }
    }
    
    (*selected).setFirstMove();
}


bool Board::movePiece(Square& clicked, bool blackTurn, Piece* selected)
{       
    // Moving selected piece - must have clicked on a valid square 
    if (selected && isLegalMove(clicked, getMoves(*selected, true)))
    {
        Move move{
            selected->getPosition(),
            clicked
        };
        makeMove(move,blackTurn);
        return true; // <-- actual move happened
    }

    // Clicking selected piece again
    else if (selected && selected->getPosition() == clicked)
    {
        selected->deselect();
        return false;
    }

    // Selecting a piece - deselect all, then selected our piece
    else
    {
        for (auto& piece : getPieces())
        {
            if ((piece->getPosition() == clicked) && piece->getBlack() == blackTurn)
            {
                for (auto& p : getPieces())
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


bool Board::testMoveForCheck(Move& move){
    Piece* piece = getPieceAt(move.from);
    std::unique_ptr<Piece> removed_piece = removePieceAt(move.to);

    piece->setPosition(move.to);
    
    bool result = checkIfImInCheck(piece->getBlack(), {});

    if (removed_piece){pieces.push_back(std::move(removed_piece));}   
    piece->setPosition(move.from);

    return (result);
} 


bool Board::checkIfImInCheck(bool isBlack, Square kingPosition)
{
    // Find the king
    if (!kingPosition.col){
    for (auto& piece : pieces)
    {
        if (piece->getType()==PieceType::King && piece->getBlack() == isBlack)
        {
            kingPosition = piece->getPosition();
            break;
        }
    }
    }

    // Check if any enemy piece attacks it
    for (auto& enemyPiece : pieces)
    {
        if (enemyPiece->getBlack() != isBlack)
        {
            std::vector<Move> moves = getMoves(*enemyPiece, false);

            for (Move move : moves)
            {
                if (move.to == kingPosition)
                    return true;
            }
        }
    }
    return false;
}


bool Board::hasNoLegalMoves(bool blackTurn)
{
    for (auto& piece : getPieces()){
        if (piece->getBlack() == blackTurn && getMoves(*piece, true).size()>0){
            return false;
        }
    }
    return true;
}


void Board::checkIfILost(bool blackTurn){
    // check checkmate 
    if (hasNoLegalMoves(blackTurn)){
        gameOver = true;

        if (!checkIfImInCheck(blackTurn, {}))
            gameOverMessage = "Stalemate - draw";
        else if (blackTurn)
            gameOverMessage = "White has won!";
        else
            gameOverMessage = "Black has won!";
    }
}


void Board::detectIfPromotion(Piece& piece){
    if (piece.getType() == PieceType::Pawn)
{
    if ((!piece.getBlack() && piece.getPosition().row == 0) ||
        ( piece.getBlack() && piece.getPosition().row == 7))
    {
        waitingForPromotion = true;
        pawnToPromote = &piece;
    }
}
}


void Board::handlePromotionClick(Square clicked)
{       
    // Queen button
    if (clicked == Square{4, 2})
    {
        finishPromotion(PieceType::Queen);
    }

    // Rook button
    else if (clicked == Square{4, 3})
    {
        finishPromotion(PieceType::Rook);
    }

    // Bishop button
    else if (clicked == Square{4, 4})
    {
        finishPromotion(PieceType::Bishop);
    }

    // Knight button
    else if (clicked == Square{4, 5})
    {
        finishPromotion(PieceType::Knight);
    }
}


void Board::finishPromotion(PieceType type)
{
    if (!pawnToPromote)return;

    Square pos = pawnToPromote->getPosition();
    bool black = pawnToPromote->getBlack();

    removePieceAt(pos);
    addPiece(type, pos, black);

    waitingForPromotion = false;
    pawnToPromote = nullptr;
}


void Board::addPiece(PieceType type, Square pos, bool black){
    removePieceAt(pos);

    switch (type)
    {
    case PieceType::Queen:
        pieces.push_back(std::make_unique<Queen>(pos, texture, black));
        break;

    case PieceType::Rook:
        pieces.push_back(std::make_unique<Rook>(pos, texture, black));
        break;

    case PieceType::Bishop:
        pieces.push_back(std::make_unique<Bishop>(pos, texture, black));
        break;

    case PieceType::Knight:
        pieces.push_back(std::make_unique<Knight>(pos, texture, black));
        break;

    case PieceType::Pawn:
        pieces.push_back(std::make_unique<Pawn>(pos, texture, black));
        break;

    case PieceType::King:
        pieces.push_back(std::make_unique<King>(pos, texture, black));
        break;
    }
}


int Board::canCastle(bool isBlack){    
    int row;
    int sum=0;
    if (isBlack){row=0;}
    else {row=7;}
    Piece* king = getPieceAt({row,4});
    Piece* rook = getPieceAt({row,0});
    
    if (!king || !rook) {
        return sum;
    }

    if (!(*king).getFirstMove()||!(*rook).getFirstMove()){
        return sum;
    }

    if ((!checkIfImInCheck(isBlack,{row,2})
        &&!checkIfImInCheck(isBlack,{row,3})
        &&!checkIfImInCheck(isBlack,{row,4})) 
        &&(!getPieceAt({row,1})
            &&!getPieceAt({row,2})
            &&!getPieceAt({row,3})))
            {sum+=1;}
    
    if ((!checkIfImInCheck(isBlack,{row,4})
        &&!checkIfImInCheck(isBlack,{row,5})
        &&!checkIfImInCheck(isBlack,{row,6})) 
        &&(!getPieceAt({row,5})
            &&!getPieceAt({row,6})))
            {sum+=2;}
    
    return sum;
}