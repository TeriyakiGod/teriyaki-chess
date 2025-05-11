#ifndef BOARD_H
#define BOARD_H

#include "chess.h"
#include <unordered_map>
#include <string>
#include <iostream>

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

class Board {
public:
    static int square[64];
    static Move lastMove;
    Board();
    static void loadPositionFromFEN(const std::string& fen);
    static void movePiece(int fromFile, int fromRank, int toFile, int toRank);
    static int getPiece(int square);
    static void setPiece(int square, int piece);
};

#endif // BOARD_H