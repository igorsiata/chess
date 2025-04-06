#pragma once

#include "game_logic/piece.hpp"
#include "game_logic/move_type.hpp"

class Rook : public Piece
{
public:
    Rook(const Position position, const bool isWhite) : Piece(position, isWhite){};
    void findPossibleMoves(const PiecesMap &allPieces) override;
private:
};