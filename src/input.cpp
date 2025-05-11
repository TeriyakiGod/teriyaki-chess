#include "input.h"
#include "video.h"

//define default values
bool Input::dragging = false;
int Input::draggedPiece = Piece::NONE;
int Input::startSquare = -1;
int Input::mouseX = 0;
int Input::mouseY = 0;
int Input::cursorX = 0;
int Input::cursorY = 0;
bool Input::keyboardSelecting = false;

void Input::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        handleMouseButtonDown(event.button);
    } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        handleMouseButtonUp(event.button);
    } else if (event.type == SDL_MOUSEMOTION) {
        handleMouseMotion(event.motion);
    } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        handleWindowResize();
    } else if (event.type == SDL_KEYDOWN) {
        handleKeyDown(event.key);
    }
}

void Input::handleMouseButtonDown(const SDL_MouseButtonEvent& button) {
    int x = button.x;
    int y = button.y;

    int square = getSquareFromMouse(x, y);
    if (square >= 0 && Board::getPiece(square) != Piece::NONE) {
        dragging = true;
        draggedPiece = Board::getPiece(square);
        startSquare = square;
    }
}

void Input::handleMouseButtonUp(const SDL_MouseButtonEvent& button) {
    if (dragging) {
        int x = button.x;
        int y = button.y;

        int square = getSquareFromMouse(x, y);
        if (square >= 0) {
            Board::movePiece(startSquare % 8, startSquare / 8, square % 8, square / 8);
        }

        dragging = false;
        draggedPiece = Piece::NONE;
        startSquare = -1;
    }
}

void Input::handleMouseMotion(const SDL_MouseMotionEvent& motion) {
    mouseX = motion.x;
    mouseY = motion.y;
    int square = getSquareFromMouse(mouseX, mouseY);
    if (square >= 0) {
        cursorX = square % 8;
        cursorY = square / 8;
    }
}

void Input::handleWindowResize() {
    Video::handleWindowResize();
}

void Input::handleKeyDown(const SDL_KeyboardEvent& key) {
    switch (key.keysym.sym) {
        case SDLK_f:
            Video::switchFullscreen();
            break;
        case SDLK_UP:
            if (cursorY > 0) cursorY--;
            break;
        case SDLK_DOWN:
            if (cursorY < 7) cursorY++;
            break;
        case SDLK_LEFT:
            if (cursorX > 0) cursorX--;
            break;
        case SDLK_RIGHT:
            if (cursorX < 7) cursorX++;
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            selectWithKeyboard();
            break;
        default:
            break;
    }
}

void Input::selectWithKeyboard() {
    int square = cursorY * 8 + cursorX;
    if (!keyboardSelecting) {
        if (square >= 0 && Board::getPiece(square) != Piece::NONE) {
            dragging = true;
            draggedPiece = Board::getPiece(square);
            startSquare = square;
            keyboardSelecting = true;
        }
    } else {
        if (square >= 0) {
            Board::movePiece(startSquare % 8, startSquare / 8, cursorX, cursorY);
        }
        dragging = false;
        draggedPiece = Piece::NONE;
        startSquare = -1;
        keyboardSelecting = false;
    }
}

int Input::getSquareFromMouse(int x, int y) {
    SDL_Renderer* renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
    int squareSize = std::min(rendererWidth, rendererHeight);
    int offsetX = (rendererWidth - squareSize) / 2;
    int offsetY = (rendererHeight - squareSize) / 2;
    x -= offsetX;
    y -= offsetY;

    if (x < 0 || y < 0 || x >= squareSize || y >= squareSize) {
        return -1; // Outside the board
    }

    int row = y / (squareSize / 8);
    int col = x / (squareSize / 8);
    return row * 8 + col;
}