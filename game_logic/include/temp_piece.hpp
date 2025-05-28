#pragma once
#include <cstdint>
#include <vector>
#include <map>

typedef uint8_t Position;

enum MoveType
{
    OUT,
    EMPTY,
    CAPTURE,
    ALLY,
    CHECK,
    CASTLE_QUEENSIDE,
    CASTLE_KINGSIDE,
    ENPASSANT

};

struct Move
{
    Position startPosition;
    Position endPosition;
    MoveType moveType;

    Move() = default;
    Move(Position sP, Position eP, MoveType mT) : startPosition(sP), endPosition(eP), moveType(mT) {}

    bool operator==(const Move &other) const
    {
        return endPosition == other.endPosition && moveType == other.moveType && startPosition == other.startPosition;
    }
};

enum PieceType
{
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN
};

struct Piece
{
    Position posit;
    PieceType pieceType;
    bool isWhite;
};

typedef std::vector<Move> MoveList;
typedef std::map<Position, Piece> PiecesMap;

namespace MoveGenerator
{
    MoveList generateMoves(const Piece &piece, const PiecesMap &piecesMap);
};