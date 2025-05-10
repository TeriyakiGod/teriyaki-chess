#ifndef CHESS_H
#define CHESS_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <iostream>

#define COLOR_WHITE SDL_Color{222, 184, 135, 255}
#define COLOR_BLACK SDL_Color{139, 69, 19, 255}

class Chess {
public:
    Chess();
    void drawBoard(SDL_Renderer* renderer);
};
#endif // CHESS_H