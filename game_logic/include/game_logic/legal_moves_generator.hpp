#pragma once

#include <gtest/gtest.h>
#include "pieces/piece.hpp"
#include "game_logic/game_state.hpp"
#include "pieces/king.hpp"
#include "pieces/knight.hpp"

class LegalMovesGenerator
{
public:
    std::vector<Move> generateLegalMoves(GameState &gameState);

private:
    std::vector<Move> m_legalMoves;

    void filterIllegallMoves(std::vector<Move> &LegalMovesGenerator, GameState &gameState);
    void addCastles(GameState &gameState);
    void addCastleQueenside(GameState &gameState);
    void addCastleKingside(GameState &gameState);
    void addEnPassant(GameState &gameState);

    Position getKingPosition(const GameState &gameState);
    bool isKingUnderAttack(const Position &kingPosition, GameState &gameState);
    bool isCheckAfterMove(const Move &move, GameState &gameState);
    bool isSquareEmpty(const Position &position, const GameState &gameState);
    FRIEND_TEST(TestLegalMovesGenerator, GetKingPosition);
};