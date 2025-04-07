#pragma once

#include "pieces/piece.hpp"
#include <array>

class SlidingPiece : public Piece
{
public:
    SlidingPiece(const Position position, const bool isWhite) : Piece(position, isWhite) {};
    void findPossibleMoves(const PiecesMap &allPieces) override;
protected:
    int m_numberOfDirections;
    std::array<int, 8> m_directionX;
    std::array<int, 8>  m_directionY;
};