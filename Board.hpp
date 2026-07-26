#pragma once

#include <vector>
#include <memory>
#include "Piece.hpp"

class Board
{
public:
    Board();

    void draw(sf::RenderWindow& window, bool blackTurn);
    std::vector<std::unique_ptr<Piece>>& getPieces();
    void set_potential_moves(const std::vector<Move>& new_moves);

    std::vector<Move> getSlidingMoves(Piece& piece, bool careAboutCheck);
    std::vector<Move> getNormalMoves(Piece& piece, bool careAboutCheck);
    std::vector<Move> getPawnMoves(Piece& piece, bool careAboutCheck);
    std::vector<Move> getMoves(Piece& piece, bool careAboutCheck);

    bool inBounds(Square& current);
    Piece* getPieceAt(Square current);
    std::unique_ptr<Piece> removePieceAt(Square square);

    bool movePiece(Square& clicked, bool blackTurn, Piece* selected);
    
    bool testMoveForCheck(Move& move);
    bool checkIfImInCheck(bool isBlack,Square kingPosition = {-1, -1});

    void decideWhetherToAddMove(Move& newMove, std::vector<Move>& moves, bool careAboutCheck);

    bool hasNoLegalMoves(bool blackTurn);
    void checkIfILost(bool blackTurn);

    void checkDiag(Move newMove, bool& careAboutCheck, std::vector<Move>& moves);


    void detectIfPromotion(Piece& piece, Move& newMove);
    bool handlePromotionClick(Square clicked, bool& blackTurn);
    void addPiece(PieceType type, Square pos, bool black);
    
    bool isWaitingToPromote(){
        return waitingForPromotion;
    }
    void setGameOver(){
        gameOver = true;
    }
    void setDrawMessage(){
        gameOverMessage = "Draw";
    }
    
    std::vector<Piece*> enPassant(Piece& piece, std::vector<Move>& moves, int& direction);
    int canCastle(bool isBlack);
    void handleCastling(Piece& piece, std::vector<Move>& moves);

    std::vector<Move> getAllLegalMoves(bool black);
    void makeMove(Move& move, bool& blackTurn);

    Move makeRandomMove(bool black);

private:
    sf::Texture texture;
    std::vector<std::unique_ptr<Piece>> pieces;
    std::vector<Move> potential_moves;

    bool gameOver = false;
    std::string gameOverMessage;

    sf::Font font;

    bool waitingForPromotion = false;

    Move pendingMove;
};