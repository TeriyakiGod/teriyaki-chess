#include "video.h"
#include "input.h"
#include "chess.h"

int main(int argc, char* argv[]) {

    if (Video::init() < 0) {
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--fullscreen") == 0) {
            Video::setFullscreen(true);
            break;
        }
    }

    Chess chess;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            } else {
                Input::handleEvent(event);
            }
        }
        Video::draw();
    }

    Video::cleanup();
    SDL_Quit();

    return 0;
}
