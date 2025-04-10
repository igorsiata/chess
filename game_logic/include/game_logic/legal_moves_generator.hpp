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
    GameState m_gameState;
    std::vector<Move> m_legalMoves;

    void filterIllegallMoves(std::vector<Move> &LegalMovesGenerator);
    void addCastles();
    void addCastleQueenside();
    void addCastleKingside();
    void addEnPassant();

    Position getKingPosition();
    bool isKingUnderAttack(const Position &kingPosition, const GameState &gameStat);
    bool isCheckAfterMove(const Move &move);
    bool isSquareEmpty(const Position &position);
    FRIEND_TEST(TestLegalMovesGenerator, GetKingPosition);
};