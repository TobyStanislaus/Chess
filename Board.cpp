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
            move.col * 100.f + 50.f,
            move.row * 100.f + 50.f
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


void Board::decideWhetherToAddMove(Piece& piece, Square& newPos, std::vector<Square>& moves, bool careAboutCheck){
    if (careAboutCheck){
        if (!testMoveForCheck(piece, newPos)){
            moves.push_back(newPos);
        }
    }else{
        moves.push_back(newPos);
    }
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

            if (otherPiece == nullptr){ //No piece
                decideWhetherToAddMove(piece, newPos, moves, careAboutCheck);
                
            } else if (otherPiece->getBlack() == piece.getBlack()) // piece on same team
                break;
            else{ // piece on other team
                decideWhetherToAddMove(piece, newPos, moves, careAboutCheck);
                break;
            }
        }
    }

    return moves;
}


void Board::handleCastling(Piece& piece, std::vector<Square>& moves){
    bool black = piece.getBlack();
    int success = canCastle(black);
    if (success==1){moves.push_back(Square{piece.getPosition().row, 2});}
    else if (success==2){moves.push_back(Square{piece.getPosition().row, 6});}
    else if (success==3){moves.push_back(Square{piece.getPosition().row, 2});
                            moves.push_back(Square{piece.getPosition().row, 6});}
}


std::vector<Square> Board::getNormalMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Square> moves;

    if (careAboutCheck && piece.getType() == PieceType::King){
        handleCastling(piece, moves);
    }

    for (Square newPos : piece.getDirections())
    {
        Piece* otherPiece = getPieceAt(newPos);

        if (!inBounds(newPos)){continue;}
        else if (otherPiece == nullptr || otherPiece->getBlack() != piece.getBlack())
        { 
            if (otherPiece == nullptr && careAboutCheck){ // nothing there and care about check
                if (!testMoveForCheck(piece, newPos)){
                    moves.push_back(newPos);
                }
            } else { // different team or do not care about check
                decideWhetherToAddMove(piece, newPos, moves, careAboutCheck);
            }
        }
    }

    return moves;
}



void Board::checkDiag(Square& currPos, Piece& piece, bool& careAboutCheck, std::vector<Square>& moves){
    Piece* diagPiece = getPieceAt(currPos);

    if (diagPiece != nullptr &&
        diagPiece->getBlack() != piece.getBlack())
    {
        if (careAboutCheck){
            if (!testMoveForCheck(piece, currPos)){
                moves.push_back(currPos);
            }
        } else{
            decideWhetherToAddMove(piece, currPos, moves, careAboutCheck);
        }
    }
}


std::vector<Piece*> Board::enPassant(Piece& piece, std::vector<Square>& moves, int& direction){
    int row = piece.getPosition().row;
    int col = piece.getPosition().col;
    Piece* leftPiece = getPieceAt(Square{row, col-1});
    Piece* rightPiece = getPieceAt(Square{row, col+1});
           

    if ((piece.getBlack()&&row==4)||(!piece.getBlack()&&row==3)){
        if (leftPiece && leftPiece->canEnPassant){
            moves.push_back(Square{row+direction,col-1});
        }
        if (rightPiece && rightPiece->canEnPassant){
            moves.push_back(Square{row+direction,col+1});
        }
    }
    return {leftPiece, rightPiece};
}


std::vector<Square> Board::getPawnMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Square> moves;

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


        if (!inBounds(newPos)){continue;}
        if (forwardPiece == nullptr)
        {
            if (careAboutCheck){
                if (!testMoveForCheck(piece, newPos)){
                    moves.push_back(newPos);
                }
            } else{
                decideWhetherToAddMove(piece, newPos, moves, careAboutCheck);
                
            }
        }

        if (forwardPiece){break;}
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


bool Board::isLegalMove(Square& clicked, std::vector<Square> moves)
{
    for (auto move : moves)
    {
        if (move == clicked)
            return true;
    }

    return false;
}


bool Board::movePiece(Board& board, Square& clicked, bool blackTurn, Piece* selected)
{       
    // Moving selected piece - must have clicked on a valid square 
    if (selected && isLegalMove(clicked, getMoves(*selected, true)))
    {
        Piece* otherPiece = board.getPieceAt(clicked);
        int differenceRow = clicked.row - selected->getPosition().row;
        int differenceCol = clicked.col - selected->getPosition().col;
        
        if (otherPiece)
        {
            board.removePieceAt(clicked);
        } else{
            if (clicked.row != selected->getPosition().row && 
                clicked.col != selected->getPosition().col &&
                ((*selected).getType() == PieceType::Pawn)){
                    board.removePieceAt(Square{selected->getPosition().row, 
                                        clicked.col});
            }
        }

        selected->setPosition(clicked);
        selected->deselect();

        handleLoss(blackTurn);
        for (auto& piece : board.getPieces())
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

    std::unique_ptr<Piece> removed_piece = removePieceAt(newPos);
    piece.setPosition(newPos);
    
    bool result = checkForCheck(piece.getBlack(), {});

    if (removed_piece){pieces.push_back(std::move(removed_piece));}   
    piece.setPosition(originalPosition);

    return (result);
} 


bool Board::checkForCheck(bool isBlack, Square kingPosition)
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


bool Board::areUCheckMated(bool blackTurn)
{
    for (auto& piece : getPieces()){
        if (piece->getBlack() != blackTurn && getMoves(*piece, true).size()>0){
            return false;
        }
    }
    return true;
}


void Board::handleLoss(bool blackTurn){
        // check checkmate 
    if (areUCheckMated(blackTurn)){
        gameOver = true;

        if (!checkForCheck(!blackTurn, {}))
            gameOverMessage = "Stalemate - draw";
        else if (blackTurn)
            gameOverMessage = "Black has won!";
        else
            gameOverMessage = "White has won!";
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

    if ((!checkForCheck(isBlack,{row,2})
        &&!checkForCheck(isBlack,{row,3})
        &&!checkForCheck(isBlack,{row,4})) 
        &&(!getPieceAt({row,1})
            &&!getPieceAt({row,2})
            &&!getPieceAt({row,3})))
            {sum+=1;}
    
    if ((!checkForCheck(isBlack,{row,4})
        &&!checkForCheck(isBlack,{row,5})
        &&!checkForCheck(isBlack,{row,6})) 
        &&(!getPieceAt({row,5})
            &&!getPieceAt({row,6})))
            {sum+=2;}
    
    return sum;
}