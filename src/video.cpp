#include "video.h"

SDL_Window* Video::window = nullptr;
SDL_Renderer* Video::renderer = nullptr;
int Video::squareSize = 0;
int Video::offsetX = 0;
int Video::offsetY = 0;
bool Video::fullscreen = false;
std::unordered_map<int, SDL_Texture*> Video::pieceTextures;

int Video::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window = SDL_CreateWindow("Chess Board", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, windowFlags);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    loadPieceTextures();
    return 0;
};

void Video::drawSquare(int file, int rank, SDL_Color color) {
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
    int SQUARE_SIZE = std::min(rendererWidth, rendererHeight) / 8;
    SDL_Rect square = { file * SQUARE_SIZE, rank * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &square);
}

void Video::drawChessBoard() {
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            // Check if last move square
            if (file == Board::lastMove.startSquare % 8 && rank == Board::lastMove.startSquare / 8) {
                drawSquare(file, rank, COLOR_LIGHT_ORANGE);
                continue;
            }
            if (file == Board::lastMove.targetSquare % 8 && rank == Board::lastMove.targetSquare / 8) {
                drawSquare(file, rank, COLOR_DARK_ORANGE);
                continue;
            }
            bool isWhiteSquare = (file + rank) % 2 == 0;
            SDL_Color color = isWhiteSquare ? COLOR_WHITE : COLOR_BLACK;
            drawSquare(file, rank, color);
        }
    }
}

void Video::drawPiece(int piece, int file, int rank) {
    SDL_Texture* texture = pieceTextures[piece];
    if (texture) {
        int rendererWidth, rendererHeight;
        SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
        int SQUARE_SIZE = std::min(rendererWidth, rendererHeight) / 8;
        SDL_Rect destRect = { file * SQUARE_SIZE, rank * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }
};

void Video::drawPieces() {
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            int piece = Board::getPiece(file + rank * 8);
            if (piece != Piece::NONE) {
                drawPiece(piece, file, rank);
            }
        }
    }
};

void Video::loadPieceTextures() {
    std::string path = "assets/textures/pieces.png";
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        SDL_Log("Failed to load texture %s: %s", path.c_str(), IMG_GetError());
        return;
    }

    // Ensure the surface has an alpha channel
    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface); // Free the original surface
    if (!optimizedSurface) {
        SDL_Log("Failed to optimize surface: %s", SDL_GetError());
        return;
    }

    // Create a texture from the optimized surface
    SDL_Texture* fullTexture = SDL_CreateTextureFromSurface(renderer, optimizedSurface);
    SDL_FreeSurface(optimizedSurface); // Free the optimized surface
    if (!fullTexture) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return;
    }

    // Enable blending for the full texture
    SDL_SetTextureBlendMode(fullTexture, SDL_BLENDMODE_BLEND);

    // Load black pieces (top row)
    for (int pieceType = Piece::KING; pieceType <= Piece::PAWN; ++pieceType) {
        SDL_Rect srcRect = { (pieceType - 1) * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE }; // Top row
        SDL_Texture* pieceTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, PIECE_SIZE, PIECE_SIZE);
        SDL_SetTextureBlendMode(pieceTexture, SDL_BLENDMODE_BLEND); // Enable blending for the piece texture
        SDL_SetRenderTarget(renderer, pieceTexture);
        SDL_RenderCopy(renderer, fullTexture, &srcRect, nullptr);
        SDL_SetRenderTarget(renderer, nullptr);
        pieceTextures[Piece::BLACK | pieceType] = pieceTexture;
    }

    // Load white pieces (bottom row)
    for (int pieceType = Piece::KING; pieceType <= Piece::PAWN; ++pieceType) {
        SDL_Rect srcRect = { (pieceType - 1) * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, PIECE_SIZE }; // Bottom row
        SDL_Texture* pieceTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, PIECE_SIZE, PIECE_SIZE);
        SDL_SetTextureBlendMode(pieceTexture, SDL_BLENDMODE_BLEND); // Enable blending for the piece texture
        SDL_SetRenderTarget(renderer, pieceTexture);
        SDL_RenderCopy(renderer, fullTexture, &srcRect, nullptr);
        SDL_SetRenderTarget(renderer, nullptr);
        pieceTextures[Piece::WHITE | pieceType] = pieceTexture;
    }

    // Clean up
    SDL_DestroyTexture(fullTexture);
}

void Video::cleanupPieceTextures() {
    for (auto& [piece, texture] : pieceTextures) {
        SDL_DestroyTexture(texture);
    }
    pieceTextures.clear();
}

void Video::handleWindowResize() {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    squareSize = std::min(windowWidth, windowHeight);
    offsetX = (windowWidth - squareSize) / 2;
    offsetY = (windowHeight - squareSize) / 2;
    SDL_Rect viewport = { offsetX, offsetY, squareSize, squareSize };
    SDL_RenderSetViewport(renderer, &viewport);
}

void Video::setFullscreen(bool fullscreen) {
    if (fullscreen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(window, 0);
    }
}

void Video::switchFullscreen() {
    fullscreen = !fullscreen;
    setFullscreen(fullscreen);
    handleWindowResize();
}

void Video::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawChessBoard();
    drawPieces();
    SDL_RenderPresent(renderer);
}

void Video::cleanup() {
    cleanupPieceTextures();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}