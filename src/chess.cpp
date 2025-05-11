#include "chess.h"

bool Piece::isType(int piece, int type) {
    return (piece & (type | Piece::WHITE | Piece::BLACK)) == type;
}

bool Piece::isColor(int piece, int color) {
    return (piece & color) == color;
}

bool Piece::isSlidingPiece(int piece) {
    return (piece & (KING | QUEEN | BISHOP | ROOK)) != 0;
}

const int Chess::directionOffsets[8] = { 8, -8, -1, 1, 7, -7, -9, 9 };

std::array<std::array<int, 64>, 8> Chess::initializeNumSquaresToEdge() {
    std::array<std::array<int, 64>, 8> data{};

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int numNorth = 7 - rank;
            int numSouth = rank;
            int numEast = 7 - file;
            int numWest = file;

            int squareIndex = rank * 8 + file;

            // Order of directions for numSquaresToEdge:
            // 0:N, 1:S, 2:E, 3:W, 4:NE, 5:NW, 6:SE, 7:SW
            data[0][squareIndex] = numNorth;
            data[1][squareIndex] = numSouth;
            data[2][squareIndex] = numEast;
            data[3][squareIndex] = numWest;
            data[4][squareIndex] = std::min(numNorth, numEast);
            data[5][squareIndex] = std::min(numNorth, numWest);
            data[6][squareIndex] = std::min(numSouth, numEast);
            data[7][squareIndex] = std::min(numSouth, numWest);
        }
    }
    return data;
}

const std::array<std::array<int, 64>, 8> Chess::numSquaresToEdge = Chess::initializeNumSquaresToEdge();
int Chess::colorToMove = Piece::WHITE;
std::vector<Move> Chess::moves = {};

Chess::Chess() {
    Board::loadPositionFromFEN(START_FEN);
}

void Chess::generateMoves() {
    moves.clear();
    for (int startSquare = 0; startSquare < 64; ++startSquare) {
        int piece = Board::getPiece(startSquare);
        if (Piece::isColor(piece, colorToMove)) {
            if (Piece::isSlidingPiece(piece)) {
                Chess::generateSlidingMoves(startSquare, piece);
            }
        }
    }
}
    
void Chess::generateSlidingMoves(int startSquare, int piece) {
    int startDirIndex = (Piece::isType(piece, Piece::BISHOP) ? 0 : 4);
    int endDirIndex = (Piece::isType(piece, Piece::ROOK) ? 4 : 8);
    for (int directionIndex = startDirIndex; directionIndex < endDirIndex; ++directionIndex) {
        for (int n = 1; n <= numSquaresToEdge[directionIndex][startSquare]; ++n) {
            int targetSquare = startSquare + directionOffsets[directionIndex] * (n+1);
            int targetPiece = Board::getPiece(targetSquare);
            
            //Blocked by friendly piece
            if (Piece::isColor(targetPiece, colorToMove)) {
                break;
            }

            moves.push_back({ startSquare, targetSquare });

            //Blocked by enemy piece
            if (Piece::isColor(targetPiece, Piece::WHITE | Piece::BLACK)) {
                break;
            }
        }
    }
}