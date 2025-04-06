#pragma once

#include "game_logic/piece.hpp"
#include "game_logic/move_type.hpp"

class Queen : public Piece
{
public:
    Queen(const Position position, const bool isWhite) : Piece(position, isWhite){};
    void findPossibleMoves(const PiecesMap &allPieces) override;
private:
};