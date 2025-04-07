#pragma once

#include "pieces/piece.hpp"

class Knight : public Piece
{
public:
    Knight(const Position position, const bool isWhite) : Piece(position, isWhite){};
    void findPossibleMoves(const PiecesMap &allPieces) override;
    char getCharRepresentation()override {return isWhite() ? 'N' : 'n';}
private:
};