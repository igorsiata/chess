#pragma once

#include "pieces/sliding_piece.hpp"

class Queen : public SlidingPiece
{
public:
    Queen(const Position position, const bool isWhite) : SlidingPiece(position, isWhite)
    {
        m_numberOfDirections = 8;
        m_directionX = {1, 1, 1, -1, -1, -1, 0, 0};
        m_directionY = {1, 0, -1, 1, 0, -1, 1, -1};
    };
    char getCharRepresentation()override {return isWhite() ? 'Q' : 'q';}
};
