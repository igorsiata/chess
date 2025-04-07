#pragma once
#include <vector>
#include <typeinfo>
#include "pieces/piece.hpp"
#include <string>

struct GameState{
    PiecesMap piecesMap;
    bool isWhiteToMove;
    std::vector<Position> availableCastles;
    bool isEnPassantPossible;
    Position enPassantSquare;
};
