#include <gtest/gtest.h>
#include "game_logic/legal_moves_generator.hpp"
#include "pieces/bishop.hpp"
#include "pieces/queen.hpp"
#include "helper_test_functions.hpp"

TEST(TestLegalMovesGenerator, GetKingPosition)
{
    GameState gameState;
    std::string positionFEN = "8/2K5/2B5/8/8/5k2/8/8 w";
    gameState.loadGameStateFromFEN(positionFEN);
    LegalMovesGenerator legalMovesGenerator;
    legalMovesGenerator.generateLegalMoves(gameState);

    EXPECT_EQ(legalMovesGenerator.getKingPosition(gameState), Position(3, 2));
}

TEST(TestLegalMovesGenerator, KingMoves1)
{
    GameState gameState;
    std::string positionFEN = "8/2K5/2b5/8/8/5k2/8/8 w";
    gameState.loadGameStateFromFEN(positionFEN);
    LegalMovesGenerator legalMovesGenerator;

    std::vector<Move> legalMoves = legalMovesGenerator.generateLegalMoves(gameState);

    EXPECT_EQ(legalMoves.size(), 6);
    EXPECT_FALSE(moveListContainsMove(legalMoves, {{3,2}, {2,2}, EMPTY}));
    EXPECT_FALSE(moveListContainsMove(legalMoves, {{3,2}, {4,2}, EMPTY}));
    EXPECT_TRUE(moveListContainsMove(legalMoves, {{3,2}, {3,3}, CAPTURE}));
}

TEST(TestLegalMovesGenerator, PieceCapture)
{
    GameState gameState;
    std::string positionFEN = "8/8/7Q/3B4/4b3/5k1K/8/8 b";
    gameState.loadGameStateFromFEN(positionFEN);
    LegalMovesGenerator legalMovesGenerator;

    std::vector<Move> legalMoves = legalMovesGenerator.generateLegalMoves(gameState);

    EXPECT_EQ(legalMoves.size(), 3);
    EXPECT_TRUE(moveListContainsMove(legalMoves, {{6,6}, {6,7}, EMPTY}));
    EXPECT_TRUE(moveListContainsMove(legalMoves, {{6,6}, {5,7}, EMPTY}));
    EXPECT_TRUE(moveListContainsMove(legalMoves, {{5,5}, {4,4}, CAPTURE}));
}

TEST(TestLegalMovesGenerator, DiscoveredCheck)
{
    GameState gameState;
    std::string positionFEN = "8/8/8/5Q2/5b2/5k2/8/8 b";
    gameState.loadGameStateFromFEN(positionFEN);
    LegalMovesGenerator legalMovesGenerator;

    std::vector<Move> legalMoves = legalMovesGenerator.generateLegalMoves(gameState);
    EXPECT_EQ(legalMoves.size(), 5);
    EXPECT_FALSE(moveListContainsMove(legalMoves, {{6,5}, {5,4}, EMPTY}));
}

TEST(TestLegalMovesGenerator, StartPosition)
{
    GameState gameState;
    std::string positionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
    gameState.loadGameStateFromFEN(positionFEN);
    LegalMovesGenerator legalMovesGenerator;

    std::vector<Move> legalMoves = legalMovesGenerator.generateLegalMoves(gameState);
    EXPECT_EQ(legalMoves.size(), 20);
}