#include "input.h"
#include "video.h"

Input::Input(Board& board)
    : board(board), dragging(false), draggedPiece(Piece::NONE), startSquare(-1), mouseX(0), mouseY(0) {}

void Input::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;

        int square = getSquareFromMouse(x, y);
        if (square >= 0 && board.getPiece(square) != Piece::NONE) {
            dragging = true;
            draggedPiece = board.getPiece(square);
            startSquare = square;
            board.setPiece(square, Piece::NONE); // Temporarily remove the piece from the board
        }
    } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        if (dragging) {
            int x = event.button.x;
            int y = event.button.y;

            int square = getSquareFromMouse(x, y);
            if (square >= 0) {
                board.setPiece(square, draggedPiece); // Place the piece on the new square
            } else {
                board.setPiece(startSquare, draggedPiece); // Return the piece to its original square
            }

            dragging = false;
            draggedPiece = Piece::NONE;
            startSquare = -1;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        Video::handleWindowResize();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f) {
        Video::switchFullscreen();
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