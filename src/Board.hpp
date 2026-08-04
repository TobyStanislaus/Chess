#pragma once

#include <vector>
#include <memory>
#include "pieces/Piece.hpp"
#include "NNEvaluator.hpp"

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
    void addPiece(PieceType type, Square pos, bool black, bool firstMove);
    
    bool isWaitingToPromote(){
        return waitingForPromotion;
    }
    void setGameOver(){
        gameOver = true;
    }
    bool getGameOver(){
        return gameOver;
    }
    void setDrawMessage(){
        gameOverMessage = "Draw";
    }
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }
    
    std::vector<Piece*> enPassant(Piece& piece, std::vector<Move>& moves, int& direction);
    int canCastle(bool isBlack);
    void handleCastling(Piece& piece, std::vector<Move>& moves);

    std::vector<Move> getAllLegalMoves(bool black);
    void makeMove(Move& move, bool& blackTurn);

    Move makeRandomMove(bool black);
    bool fiftyMoveDraw();

    void undoMove(bool blackTurn);
    int evaluate();

    int minimax(int depth, bool blackTurn, int alpha, int beta);
    Move findBestMove(bool blackTurn, int depth);
    Move findBestMoveMCTS(bool blackTurn, int numSimulations=200);
private:
    sf::Texture texture;
    std::vector<std::unique_ptr<Piece>> pieces;
    std::vector<Move> potential_moves;

    bool gameOver = false;
    std::string gameOverMessage;

    sf::Font font;

    bool waitingForPromotion = false;

    Move pendingMove;
    std::vector<Move> moveHistory;
    NNEvaluator nnEvaluator{"assets/network_1.onnx"};
};