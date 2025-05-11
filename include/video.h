#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include "board.h"

#define COLOR_WHITE SDL_Color{222, 184, 135, 255}
#define COLOR_BLACK SDL_Color{139, 69, 19, 255}
#define COLOR_LIGHT_ORANGE SDL_Color{255, 165, 0, 255}
#define COLOR_DARK_ORANGE SDL_Color{255, 140, 0, 255}

#define PIECE_SIZE 16

class Video {
public:
    static int init();
    static void cleanup();
    static void draw(Board& board);
    static void drawChessBoard();
    static void drawSquare(int file, int rank, SDL_Color color);
    static void drawPiece(int piece, int file, int rank);
    static void drawPieces(Board& board);
    static void cleanupPieceTextures();
    static void loadPieceTextures();
    static void handleWindowResize();
    static void setFullscreen(bool fullscreen);
    static void switchFullscreen();

private:
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static int squareSize;
    static int offsetX;
    static int offsetY;
    static bool fullscreen;
    static std::unordered_map<int, SDL_Texture*> pieceTextures;
};

#endif // VIDEO_H