#pragma once

#include "pieces/piece.hpp"

class Pawn : public Piece
{
public:
    Pawn(const Position position, const bool isWhite) : Piece(position, isWhite){};
    void findPossibleMoves(const PiecesMap &allPieces) override;
    char getCharRepresentation()override {return isWhite() ? 'P' : 'p';}
private:
};