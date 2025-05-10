#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "board.h"

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

#endif // INPUT_H