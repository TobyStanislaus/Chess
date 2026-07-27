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
        Queen queen({4, 2}, texture, blackTurn);
        Rook rook({4, 3}, texture, blackTurn);
        Bishop bishop({4, 4}, texture, blackTurn);
        Knight knight({4, 5}, texture, blackTurn);

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


void setCapturedPiece(Move& newMove, Piece* captured){
    newMove.capturedPiece = captured;
    newMove.capturedPieceType = captured->getType();
    newMove.capturedPieceBlack = captured->getBlack();
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
            } 
            else if (otherPiece->getBlack() == piece.getBlack()) // piece on same team
                break;
            else{ // piece on other team
                setCapturedPiece(newMove, otherPiece);
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
    int row = piece.getPosition().row;
    if (success==1 || success==3)
        {moves.push_back(Move{
            piece.getPosition(),Square{row, 2},
            PieceType::Queen, false, nullptr, true, false, PieceType::King
        });}
    if (success==2 || success==3)
        {moves.push_back(Move{
            piece.getPosition(),Square{row, 6},
            PieceType::Queen, false, nullptr, true, false, PieceType::King
        });}
}


std::vector<Move> Board::getNormalMoves(Piece& piece, bool careAboutCheck)
{
    std::vector<Move> moves;

    if (careAboutCheck && piece.getType() == PieceType::King){
        handleCastling(piece, moves);
    }

    for (Square newPos : piece.getDirections())
    {
        if (!inBounds(newPos)){continue;}


        Piece* otherPiece = getPieceAt(newPos);
        Move newMove = {piece.getPosition(), newPos};

        if (otherPiece == nullptr){ 
            decideWhetherToAddMove(newMove, moves, careAboutCheck);
        }
        else if (otherPiece->getBlack() != piece.getBlack()){
            setCapturedPiece(newMove, otherPiece);
            decideWhetherToAddMove(newMove, moves, careAboutCheck);
        }
    }

    return moves;
}


void Board::checkDiag(Move newMove, bool& careAboutCheck, std::vector<Move>& moves){
    Piece* diagPiece = getPieceAt(newMove.to);
    Piece* piece = getPieceAt(newMove.from);

    if (diagPiece != nullptr && diagPiece->getBlack() != piece->getBlack())
    {
        setCapturedPiece(newMove, diagPiece);
        decideWhetherToAddMove(newMove, moves, careAboutCheck);
    }
}


std::vector<Piece*> Board::enPassant(Piece& piece, std::vector<Move>& moves, int& direction){
    int row = piece.getPosition().row;
    int col = piece.getPosition().col;
    Piece* leftPiece = getPieceAt(Square{row, col-1});
    Piece* rightPiece = getPieceAt(Square{row, col+1});

    if ((piece.getBlack()&&row==4)||(!piece.getBlack()&&row==3)){

        if (leftPiece && leftPiece->canEnPassant){
            Move m{piece.getPosition(), Square{row+direction, col-1}};
            setCapturedPiece(m, leftPiece);
            m.isEnPassant = true;
            moves.push_back(m);
        }
        if (rightPiece && rightPiece->canEnPassant){
            Move m{piece.getPosition(), Square{row+direction, col+1}};
            setCapturedPiece(m, rightPiece);
            m.isEnPassant = true;
            moves.push_back(m);
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

    Move newMove = {piece.getPosition(), currPos};
    detectIfPromotion(piece, newMove);
    checkDiag(newMove, careAboutCheck, moves);

    currPos.col-=direction*2;

    newMove = {piece.getPosition(), currPos};
    detectIfPromotion(piece, newMove);
    checkDiag(newMove, careAboutCheck, moves);

    enPassant(piece, moves, direction);
    for (Square newPos : piece.getDirections())
    {
        Piece* forwardPiece = getPieceAt(newPos);
        Move newMove = {piece.getPosition(), newPos};
        detectIfPromotion(piece, newMove);
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
            std::vector<Move> newMoves = getMoves(*piece, true);
            moves.insert(moves.end(), newMoves.begin(), newMoves.end());
        }
    }

    return moves;
}


Move Board::makeRandomMove(bool black){
    std::vector<Move> moves = getAllLegalMoves(black);

    if (moves.empty()) return {{-1,-1}, {-1,-1}};

    std::uniform_int_distribution<int> randI(0, moves.size()-1);
    Move chosenMove = moves[randI(gen)];
    if (chosenMove.isPromotion) chosenMove.promotionPiece = PieceType::Queen;
    
    return chosenMove;
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
      

void Board::makeMove(Move& move, bool& blackTurn){
    Piece* selected = getPieceAt(move.from);
    move.movedPiece = selected->getType();
    move.movedPieceFirstMoveBefore = selected->getFirstMove();

    move.hadEnPassantTarget = false;
    for (auto& piece : getPieces()){
        if (piece->canEnPassant){
            move.hadEnPassantTarget = true;
            move.enPassantTargetSquare = piece->getPosition();
            break; // only one piece can ever be eligible at once
        }
    }
 
    if (move.capturedPiece){
    removePieceAt(move.capturedPiece->getPosition());
    }


    selected->setPosition(move.to);
    selected->deselect();

    //reset all pieces to no enpassant
    for (auto& piece : getPieces()) piece->canEnPassant = false;

    if ((selected->getType() == PieceType::Pawn)){
        int diff = move.to.row - move.from.row;
        if (diff == 2 || diff == -2) selected->canEnPassant=true;
        if (move.isPromotion){
            bool isBlack = selected->getBlack();
            removePieceAt(move.to);
            addPiece(move.promotionPiece, move.to, isBlack);
            moveHistory.push_back(move);
            return;
        }
    }
  
    if (move.isCastle){
        int row = move.to.row;
        if (move.to.col == 2){
            removePieceAt(Square{row,0});
            addPiece(PieceType::Rook, {row,3}, blackTurn);
        }else if (move.to.col == 6){
            removePieceAt(Square{row,7});
            addPiece(PieceType::Rook, {row,5}, blackTurn);
        }
    }
    

    selected->setFirstMove(false);
    moveHistory.push_back(move);
} 


void Board::undoMove(bool blackTurn){
    Move move = moveHistory.back();

    if (move.isPromotion){
        removePieceAt(move.to);
        addPiece(PieceType::Pawn, move.from, blackTurn);
    } else {
        Piece* moved = getPieceAt(move.to);
        moved->setPosition(move.from);
    }

    if (move.isCastle){
        int row = move.from.row;
        if (move.to.col == 2){
            removePieceAt({row,3});
            addPiece(PieceType::Rook, {row,0}, blackTurn);
        } else if (move.to.col == 6){
            removePieceAt({row,5});
            addPiece(PieceType::Rook, {row,7}, blackTurn);
        }
    }

    if (move.capturedPiece){
        if (move.isEnPassant){
            addPiece(move.capturedPieceType, Square{move.from.row, move.to.col}, move.capturedPieceBlack);
        } else {
            addPiece(move.capturedPieceType, move.to, move.capturedPieceBlack);
        }
    }

    Piece* restored = getPieceAt(move.from);
    
    if (restored) {restored->setFirstMove(move.movedPieceFirstMoveBefore);
        restored->deselect();
    }

    // restore canEnPassant exactly as it was before this move
    for (auto& piece : getPieces()) {piece->canEnPassant = false;
                                    piece->deselect();}
    if (move.hadEnPassantTarget){
        Piece* target = getPieceAt(move.enPassantTargetSquare);
        if (target) target->canEnPassant = true;
    }

    moveHistory.pop_back();
}


bool Board::movePiece(Square& clicked, bool blackTurn, Piece* selected)
{       
    // Moving selected piece - must have clicked on a valid square 
    if (selected)
    {
        std::vector<Move> legalMoves = getMoves(*selected, true);

        for (Move& m : legalMoves)
        {
            if (m.to == clicked)
            {
                if (m.isPromotion)
                {
                    pendingMove = m;
                    waitingForPromotion = true;
                    return false; // move isn't finished yet
                }

                makeMove(m, blackTurn);
                return true;
            }
        }

        if (selected->getPosition() == clicked)
        {
            selected->deselect();
            return false;
        }
    }

   
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
    
    return false;
}


bool Board::testMoveForCheck(Move& move){
    Piece* piece = getPieceAt(move.from);
    Square capturedOriginalPos{-1,-1};

    if (move.capturedPiece){
        capturedOriginalPos = move.capturedPiece->getPosition();
        move.capturedPiece->setPosition({-99,-99});
    }

    piece->setPosition(move.to);
    bool result = checkIfImInCheck(piece->getBlack());

    piece->setPosition(move.from);
    if (move.capturedPiece) move.capturedPiece->setPosition(capturedOriginalPos);

    return (result);
} 


bool Board::checkIfImInCheck(bool isBlack, Square kingPosition)
{
    // Find the king
    if (kingPosition.row == -1 && kingPosition.col == -1){
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
    if (fiftyMoveDraw()){
        gameOver = true;
        gameOverMessage = "Draw - 50 move rule";
        return;
    }

    if (hasNoLegalMoves(blackTurn)){
        gameOver = true;

        if (!checkIfImInCheck(blackTurn))
            gameOverMessage = "Stalemate - draw";
        else if (blackTurn)
            gameOverMessage = "White has won!";
        else
            gameOverMessage = "Black has won!";
    }
}


void Board::detectIfPromotion(Piece& piece, Move& newMove){

    if ((!piece.getBlack() && newMove.to.row == 0) ||
        ( piece.getBlack() && newMove.to.row == 7))
    {
        newMove.isPromotion = true;
    }

}


bool Board::handlePromotionClick(Square clicked, bool& blackTurn)
{       
    // Queen button
    if (clicked == Square{4, 2}) pendingMove.promotionPiece = PieceType::Queen;
    else if (clicked == Square{4, 3}) pendingMove.promotionPiece = PieceType::Rook;
    else if (clicked == Square{4, 4}) pendingMove.promotionPiece = PieceType::Bishop;
    else if (clicked == Square{4, 5}) pendingMove.promotionPiece = PieceType::Knight;
    else return false;
    
    makeMove(pendingMove, blackTurn);
    waitingForPromotion = false;
    return true;
}


void Board::addPiece(PieceType type, Square pos, bool black){

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


bool Board::fiftyMoveDraw()
{
    int count = 0;
    for (auto it = moveHistory.rbegin(); it != moveHistory.rend(); ++it)
    {
        if (it->capturedPiece != nullptr || it->movedPiece == PieceType::Pawn) break;
        count++;
        if (count >= 100) return true;
    }
    return false;
}


int pieceValue(PieceType type){
    switch(type){
        case PieceType::Pawn:   return 100;
        case PieceType::Knight: return 320;
        case PieceType::Bishop: return 330;
        case PieceType::Rook:   return 500;
        case PieceType::Queen:  return 900;
        case PieceType::King:   return 20000;
    }
    return 0;
}


int Board::evaluate(){
    int score = 0;
    for (auto& piece : pieces){
        int value = pieceValue(piece->getType());
        score += piece->getBlack() ? -value : value;
    }
    return score;
}

int Board::minimax(int depth, bool blackTurn, int alpha, int beta){
    std::vector<Move> moves = getAllLegalMoves(blackTurn);

    if (moves.empty()){
        if (checkIfImInCheck(blackTurn, {-1,-1}))
            return blackTurn ? 100000 - (10-depth) : -100000 + (10-depth); // checkmate — good for the side NOT in check
        else
            return 0; // stalemate — draw
    }

    if (depth == 0){
        return evaluate();
    }

    if (blackTurn){ 
        int best = INT32_MAX;
        for (Move& m : moves){
            makeMove(m, blackTurn);
            int score = minimax(depth-1, false, alpha, beta); // make move, carry on
            undoMove(blackTurn);

            best = std::min(best, score);
            beta = std::min(beta, best);
            if (beta <= alpha) break; // prune
        }
        return best;
    } else { 
        int best = INT32_MIN;
        for (Move& m : moves){
            makeMove(m, blackTurn);
            int score = minimax(depth-1, true, alpha, beta);
            undoMove(blackTurn);

            best = std::max(best, score);
            alpha = std::max(alpha, best);
            if (beta <= alpha) break; // prune
        }
        return best;
    }
}

Move Board::findBestMove(bool blackTurn, int depth){
    std::vector<Move> moves = getAllLegalMoves(blackTurn);
    if (moves.empty()) return {{-1,-1},{-1,-1}};

    Move bestMove = moves[0];
    int bestScore = blackTurn ? INT32_MAX : INT32_MIN;

    for (Move& m : moves){
        makeMove(m, blackTurn);
        int score = minimax(depth-1, !blackTurn, INT32_MIN, INT32_MAX);
        undoMove(blackTurn);

        if (blackTurn && score < bestScore){ bestScore = score; bestMove = m; }
        if (!blackTurn && score > bestScore){ bestScore = score; bestMove = m; }
    }

    if (bestMove.isPromotion) bestMove.promotionPiece = PieceType::Queen;
    return bestMove;
}