#pragma once
#include <vector>
#include "game_logic/move.hpp"

struct GameState{
    bool isWhiteToMove;
    std::vector<Position> availableCastlesSquares;
    Position enPassantSquare;
};