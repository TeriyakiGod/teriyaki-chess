#ifndef CHESS_H
#define CHESS_H

#include <array>
#include <vector>

struct Move {
    int startSquare;
    int targetSquare;
};

#include "board.h"

class Piece {
public:
    static const int WHITE = 8;
    static const int BLACK = 16;
    
    static const int NONE = 0;
    static const int KING = 1;
    static const int QUEEN = 2;
    static const int KNIGHT = 3;
    static const int BISHOP = 4;
    static const int ROOK = 5;
    static const int PAWN = 6;

    static bool isType(int piece, int type);
    static bool isColor(int piece, int color);
    static bool isSlidingPiece(int piece);
};

class Chess {
public:
    static int colorToMove;
    static const int directionOffsets[8];
    static const std::array<std::array<int, 64>, 8> numSquaresToEdge;
    static std::vector<Move> moves;
    Chess();
    void generateMoves();
    void generateSlidingMoves(int startSquare, int piece);
private:
    static std::array<std::array<int, 64>, 8> initializeNumSquaresToEdge();
};
#endif // CHESS_H