#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "board.h"

class Input {
public:
    static void handleEvent(const SDL_Event& event);
    static void handleMouseButtonDown(const SDL_MouseButtonEvent& button);
    static void handleMouseButtonUp(const SDL_MouseButtonEvent& button);
    static void handleMouseMotion(const SDL_MouseMotionEvent& motion);
    static void handleWindowResize();
    static void handleKeyDown(const SDL_KeyboardEvent& key);

    static int getCursorX() { return cursorX; }
    static int getCursorY() { return cursorY; }

private:
    static bool dragging;
    static int draggedPiece;
    static int startSquare;
    static int mouseX, mouseY;
    static int cursorX, cursorY;
    static bool keyboardSelecting;

    static int getSquareFromMouse(int x, int y);
    static void selectWithKeyboard();
};

#endif // INPUT_H
