#include <gtest/gtest.h>
#include "game_logic/legal_moves_generator.hpp"
#include "pieces/bishop.hpp"

TEST(TestLegalMovesGenerator, GetKingPosition)
{
    GameState gameState;
    gameState.isWhiteToMove = true;
    gameState.piecesMap[Position(3, 2)] = std::make_shared<King>(Position(3, 2), true);
    gameState.piecesMap[Position(3, 3)] = std::make_shared<Bishop>(Position(3, 3), true);
    gameState.piecesMap[Position(6, 7)] = std::make_shared<King>(Position(6, 7), false);
    LegalMovesGenerator legalMovesGenerator;
    legalMovesGenerator.generateLegalMoves(gameState);
    

    EXPECT_EQ(legalMovesGenerator.getKingPosition(), Position(3, 2));
}