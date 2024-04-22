#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP

#include <utility>
#include <map>

using Position = std::pair<int, int>;

enum MoveType {
    OUT, EMPTY, CAPTURE, ALLY, CHECK, CASTLE, ENPASSAT
};

struct Move {
    Position endPosition;
    MoveType moveType;
};


#endif //CHESS_MOVE_HPP
