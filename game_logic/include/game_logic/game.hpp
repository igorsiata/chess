#pragma once

#include "pieces/piece.hpp"
#include "game_logic/game_state.hpp"
#include "game_logic/position_loader.hpp"
#include <string>
class Game
{
public:
    void makeMove(std::shared_ptr<Piece> piece, const Move move);
    void loadPosition(const std::string &positionFEN);
    PiecesMap getPiecesMap() const {return m_gameState.piecesMap;}
private:
    GameState m_gameState;
};