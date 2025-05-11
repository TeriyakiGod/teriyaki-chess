#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "board.h"

class Input {
public:
    Input(Board& board);

    void handleEvent(const SDL_Event& event);
    void handleMouseButtonDown(const SDL_MouseButtonEvent& button);
    void handleMouseButtonUp(const SDL_MouseButtonEvent& button);
    void handleMouseMotion(const SDL_MouseMotionEvent& motion);
    void handleWindowResize();
    void handleKeyDown(const SDL_KeyboardEvent& key);

private:
    Board& board;

    bool dragging;
    int draggedPiece;
    int startSquare;
    int mouseX, mouseY;
    int cursorX, cursorY;

    int getSquareFromMouse(int x, int y);
};

#endif // INPUT_H