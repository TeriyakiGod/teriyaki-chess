#include "chess.h"

Chess::Chess() {

}

void Chess::drawBoard(SDL_Renderer* renderer) {
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
    int SQUARE_SIZE = std::min(rendererWidth, rendererHeight) / 8;
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            bool isWhiteSquare = (file + rank) % 2 == 0;
            SDL_Rect square = { file * SQUARE_SIZE, rank * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
            SDL_Color color = isWhiteSquare ? COLOR_WHITE : COLOR_BLACK;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &square);
        }
    }
}
