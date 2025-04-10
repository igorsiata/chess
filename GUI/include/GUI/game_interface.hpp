#pragma once

#include "game_logic/game.hpp"
#include <typeinfo>
#include <iostream>

class GameInterface
{
public:
    GameInterface();
    std::map<Position, char> getPiecesAsChar() const;
    std::vector<Move> getPiecePossibleMoves(const Position &piecePosition);
    void movePiece(const Position &startSquare, const Position &endSquare);
private:
    Game m_game;
};
