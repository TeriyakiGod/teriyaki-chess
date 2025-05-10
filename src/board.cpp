#include "chess.h"

int Board::square[64] = { Piece::NONE };

Board::Board() {
    // Initialize the board with the starting position
    loadPositionFromFEN(START_FEN);
};

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

void Board::movePiece(int fromFile, int fromRank, int toFile, int toRank) {
    int piece = this->getPiece(fromFile + fromRank * 8);
    this->setPiece(fromFile + fromRank * 8, Piece::NONE);
    this->setPiece(toFile + toRank * 8, piece);
}

int Board::getPiece(int square) {
    return this->square[square];
}
void Board::setPiece(int square, int piece) {
    this->square[square] = piece;
}