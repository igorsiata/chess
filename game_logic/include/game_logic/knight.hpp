#pragma once

#include "game_logic/piece.hpp"
#include "game_logic/move_type.hpp"

class Knight : public Piece
{
public:
    Knight(const Position position, const bool isWhite) : Piece(position, isWhite){};
    void findPossibleMoves(const PiecesMap &allPieces) override;
private:
};