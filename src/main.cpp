#include "chess.h"
#include <cstring>

void handleWindowResize(SDL_Window* window, int& squareSize, int& offsetX, int& offsetY) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate square renderer dimensions
    squareSize = std::min(windowWidth, windowHeight);
    offsetX = (windowWidth - squareSize) / 2;
    offsetY = (windowHeight - squareSize) / 2;
}

int main(int argc, char* argv[]) {
    bool fullscreen = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--fullscreen") == 0) {
            fullscreen = true;
            break;
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    SDL_Window* window = SDL_CreateWindow("Chess Board", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, windowFlags);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Chess chess;
    bool running = true;
    SDL_Event event;

    int squareSize, offsetX, offsetY;
    handleWindowResize(window, squareSize, offsetX, offsetY); // Initial calculation

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
            running = false;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            handleWindowResize(window, squareSize, offsetX, offsetY);
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen with black
        SDL_RenderClear(renderer);

        // Set viewport to the square area
        SDL_Rect viewport = { offsetX, offsetY, squareSize, squareSize };
        SDL_RenderSetViewport(renderer, &viewport);

        chess.drawBoard(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
