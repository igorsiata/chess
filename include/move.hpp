#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP

#include <utility>

using Position = std::pair<int, int>;

enum MoveType {
    OUT, EMPTY, CAPTURE, ALLY, CHECK
};

struct Move {
    Position startPosition;
    Position endPosition;
    MoveType moveType;
};


#endif //CHESS_MOVE_HPP
