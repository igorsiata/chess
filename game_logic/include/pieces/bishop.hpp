#pragma once

#include "pieces/sliding_piece.hpp"

class Bishop : public SlidingPiece
{
public:
    Bishop(const Position position, const bool isWhite) : SlidingPiece(position, isWhite)
    {
        m_numberOfDirections = 4;
        m_directionX = {1, 1, -1, -1};
        m_directionY = {1, -1, 1, -1};
    };
    char getCharRepresentation()override {return isWhite() ? 'B' : 'b';}
private:
};