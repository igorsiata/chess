#pragma once

#include "pieces/piece.hpp"
#include "game_logic/game_state.hpp"
#include "game_logic/legal_moves_generator.hpp"
class Game
{
public:
    void makeMove(const Move move);
    void loadPositionFEN(const std::string &positionFEN);
    std::vector<Move> getPossibleMoves();
    PiecesMap getPiecesMap() const {return m_gameState.getPiecesMap();}
private:
    GameState m_gameState;
};