#pragma once

#include <gtest/gtest.h>
#include "pieces/piece.hpp"
#include "game_logic/game_state.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"

class LegalMovesGenerator
{
public:
    std::vector<Move> generateLegalMoves(const GameState &gameState);

private:
    std::vector<Move> m_legalMoves;
    GameState m_gameState;

    std::vector<Move>  generateLegalMoves();
    void filterIllegallMoves(std::vector<Move> &moves);
    Position getKingPosition();
    bool isKingUnderAttack(const Position &kingPosition, const GameState &gameState);
    bool isCheckAfterMove(const Move &move);
    void makeMove(const Move &move, GameState &gameState);
    FRIEND_TEST(TestLegalMovesGenerator, GetKingPosition);
};