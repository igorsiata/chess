#pragma once

#include "pieces/piece.hpp"

class King : public Piece
{
public:
    King(const Position position, const bool isWhite) : Piece(position, isWhite){};
    void findPossibleMoves(const PiecesMap &allPieces) override;
    char getCharRepresentation()override {return isWhite() ? 'K' : 'k';}
private:
};