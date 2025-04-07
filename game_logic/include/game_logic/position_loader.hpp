#pragma once
#include "pieces/piece.hpp"
#include "pieces/bishop.hpp"
#include "pieces/pawn.hpp"
#include "pieces/knight.hpp"
#include "pieces/rook.hpp"
#include "pieces/queen.hpp"
#include "pieces/king.hpp"
#include "game_logic/game_state.hpp"
#include <string>

class PositionLoader
{
public:
    GameState loadPositionFromFEN(const std::string &standardNotation);
private:
    GameState m_gameState;

    void loadPieces(const std::string &standardNotation);
    void loadPlayerToMove(const std::string &standardNotation);
    void loadAvailableCastles(const std::string &standardNotation);
    
};