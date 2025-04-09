#pragma once
#include "game_logic/move.hpp"
#include <vector>
#include <algorithm>

inline bool moveListContainsMove(const std::vector<Move> &vec, const Move &move)
{
    return std::find(vec.begin(), vec.end(), move) != vec.end();
}