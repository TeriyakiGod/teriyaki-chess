#ifndef BOARD_H
#define BOARD_H

#include <unordered_map>
#include <string>
#include <iostream>

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum Piece {
    // Piece types
    NONE = 0,
    KING = 1,
    QUEEN = 2,
    KNIGHT = 3,
    BISHOP = 4,
    ROOK = 5,
    PAWN = 6,
    // Piece colors
    WHITE = 8,
    BLACK = 16
};

class Board {
public:
    static int square[64];
    Board();
    void loadPositionFromFEN(const std::string& fen);
    void movePiece(int fromFile, int fromRank, int toFile, int toRank);
    int getPiece(int square);
    void setPiece(int square, int piece);
};

#endif // BOARD_H