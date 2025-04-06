#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP

#include <utility>
#include <map>
#include <cstdint>


enum MoveType {
    OUT, EMPTY, CAPTURE, ALLY, CHECK, CASTLE, ENPASSAT
    
};

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Position& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};

struct Move {
    Position endPosition;
    MoveType moveType;

    bool operator==(const Move& other) const {
        return endPosition == other.endPosition && moveType == other.moveType;
    }
};


#endif //CHESS_MOVE_HPP
