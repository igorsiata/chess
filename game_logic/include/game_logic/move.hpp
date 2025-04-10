#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP

#include <utility>
#include <map>
#include <cstdint>

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

struct Position
{
    int x;
    int y;

    Position() = default;
    Position(int x, int y) : x(x), y(y) {}

    bool operator==(const Position &other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator<(const Position &other) const
    {
        if (x == other.x)
        {
            return y < other.y;
        }
        return x < other.x;
    }

    Position operator+(const Position& other) const {
        Position result;
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        return result;
    }
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

#endif // CHESS_MOVE_HPP
