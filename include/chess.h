#ifndef CHESS_H
#define CHESS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

#define COLOR_WHITE SDL_Color{222, 184, 135, 255}
#define COLOR_BLACK SDL_Color{139, 69, 19, 255}
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
    static std::unordered_map<int, SDL_Texture*> pieceTextures;
    static int square[64];
    Board();
    void drawBoard(SDL_Renderer* renderer);
    void drawPiece(SDL_Renderer* renderer, int piece, int file, int rank);
    void drawPieces(SDL_Renderer* renderer);
    void cleanupPieceTextures();
    void loadPieceTextures(SDL_Renderer* renderer);
    void loadPositionFromFEN(const std::string& fen);
};

class Chess {
public:
    Board board;
    Chess();
};
#endif // CHESS_H