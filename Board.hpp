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
    void set_potential_moves(const std::vector<Square>& new_moves);

    std::vector<Square> getSlidingMoves(Piece& piece, bool careAboutCheck);
    std::vector<Square> getNormalMoves(Piece& piece, bool careAboutCheck);
    std::vector<Square> getPawnMoves(Piece& piece, bool careAboutCheck);
    std::vector<Square> getMoves(Piece& piece, bool careAboutCheck);

    bool inBounds(Square& current);
    Piece* getPieceAt(Square current);
    std::unique_ptr<Piece> removePieceAt(Square square);

    bool isLegalMove(Square& clicked, std::vector<Square> moves);
    bool movePiece(Board& board, Square& clicked, bool blackTurn);
    
    bool testMoveForCheck(Piece& piece, Square& dir);
    bool checkForCheck(bool isBlack,Square kingPosition);

    void decideWhetherToAddMove(Piece& piece, Square& newPos, std::vector<Square>& moves, bool careAboutCheck);

    bool areUCheckMated(bool blackTurn);
    void handleLoss(bool blackTurn);

    void checkDiag(Square& currPos, Piece& piece, bool& careAboutCheck, std::vector<Square>& moves);


    void detectIfPromotion(Piece& piece);
    void handlePromotionClick(Square clicked);
    void finishPromotion(PieceType type);
    void addPiece(PieceType type, Square pos, bool black);
    
    bool isWaitingToPromote(){
        return waitingForPromotion;
    }

    
    std::vector<Piece*> enPassant(Piece& piece, std::vector<Square>& moves, int& direction);
    int canCastle(bool isBlack);

private:
    sf::Texture texture;
    std::vector<std::unique_ptr<Piece>> pieces;
    std::vector<Square> potential_moves;

    bool gameOver = false;
    std::string gameOverMessage;

    sf::Font font;

    bool waitingForPromotion = false;
    Piece* pawnToPromote = nullptr;
};