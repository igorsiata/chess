#pragma once
#include "bitboard_operations.hpp"
#include "defs.hpp"



struct Board{
    PieceType pieces[120];
    Color sideToMove;
    Position kingsPositions[2];
    Position enPassantSquare;
    int fiftyMoveCounter;

    uint64_t positionKey;

};


int Array120To64(int index);
int Array64To120(int index);
