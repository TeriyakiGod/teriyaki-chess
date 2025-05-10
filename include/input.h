#ifndef INPUT_H
#define INPUT_H

#include "chess.h"
#include <SDL2/SDL.h>

class Input {
public:
    Input(Board& board);

    void handleEvent(const SDL_Event& event);

private:
    Board& board;

    bool dragging;
    int draggedPiece;
    int startSquare;
    int mouseX, mouseY;

    int getSquareFromMouse(int x, int y);
};

#endif