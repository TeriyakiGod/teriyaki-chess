#include "chess.h"

int Board::square[64] = { Piece::NONE };
std::unordered_map<int, SDL_Texture*> Board::pieceTextures;

Board::Board() {
    // Initialize the board with the starting position
    loadPositionFromFEN(START_FEN);
};

void Board::drawBoard(SDL_Renderer* renderer) {
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
};

void Board::drawPiece(SDL_Renderer* renderer, int piece, int file, int rank) {
    SDL_Texture* texture = pieceTextures[piece];
    if (texture) {
        int rendererWidth, rendererHeight;
        SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
        int SQUARE_SIZE = std::min(rendererWidth, rendererHeight) / 8;
        SDL_Rect destRect = { file * SQUARE_SIZE, rank * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }
};

void Board::drawPieces(SDL_Renderer* renderer) {
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            int piece = square[file + rank * 8];
            if (piece != Piece::NONE) {
                drawPiece(renderer, piece, file, rank);
            }
        }
    }
};

void Board::loadPieceTextures(SDL_Renderer* renderer) {
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

    // Define the size of each piece (16x16 pixels)
    const int PIECE_SIZE = 16;

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

void Board::cleanupPieceTextures() {
    for (auto& [piece, texture] : pieceTextures) {
        SDL_DestroyTexture(texture);
    }
    pieceTextures.clear();
}

void Board::loadPositionFromFEN(const std::string& fen) {
    auto pieceMap = std::unordered_map<char, int>{
        {'k', Piece::KING},
        {'p', Piece::PAWN},
        {'n', Piece::KNIGHT},
        {'b', Piece::BISHOP},
        {'r', Piece::ROOK},
        {'q', Piece::QUEEN}
    };

    std::string fenBoard = fen.substr(0, fen.find(' '));
    int file = 0, rank = 7;

    for (char c : fenBoard) {
        if (c == '/') {
            rank--;
            file = 0;
        } else {
            if (isdigit(c)) {
                file += c - '0'; // Skip empty squares
            } else {
                int pieceColor = (isupper(c) ? Piece::WHITE : Piece::BLACK);
                int pieceType = pieceMap[tolower(c)];
                square[file + rank * 8] = pieceColor | pieceType;
                file++;
            }
        }
    }
}