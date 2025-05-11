#include "board.h"

int Board::square[64] = { Piece::NONE };
Move Board::lastMove = Move{ -1, -1 };

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
    if (fromFile == toFile && fromRank == toRank) {
        std::cout << "Invalid move: same square" << std::endl;
        return;
    }
    int piece = Board::getPiece(fromFile + fromRank * 8);
    Board::setPiece(fromFile + fromRank * 8, Piece::NONE);
    Board::setPiece(toFile + toRank * 8, piece);
    Board::lastMove = Move{ fromFile + fromRank * 8, toFile + toRank * 8 };
    std::cout << "Moved piece from " << fromFile << "," << fromRank << " to " << toFile << "," << toRank << std::endl;
}

int Board::getPiece(int square) {
    return Board::square[square];
}
void Board::setPiece(int square, int piece) {
    Board::square[square] = piece;
}