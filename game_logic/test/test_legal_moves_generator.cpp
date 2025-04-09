#include <gtest/gtest.h>
#include "game_logic/legal_moves_generator.hpp"
#include "pieces/bishop.hpp"
#include "pieces/queen.hpp"
#include "helper_test_functions.hpp"

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

TEST(TestLegalMovesGenerator, KingMoves1)
{
    GameState gameState;
    gameState.isWhiteToMove = true;
    gameState.piecesMap[Position(3, 2)] = std::make_shared<King>(Position(3, 2), true);
    gameState.piecesMap[Position(3, 3)] = std::make_shared<Bishop>(Position(3, 3), false);
    gameState.piecesMap[Position(6, 7)] = std::make_shared<King>(Position(6, 7), false);

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
    gameState.isWhiteToMove = false;
    gameState.piecesMap[Position(6, 6)] = std::make_shared<King>(Position(6, 6), false);
    gameState.piecesMap[Position(5, 5)] = std::make_shared<Bishop>(Position(5, 5), false);
    gameState.piecesMap[Position(4, 4)] = std::make_shared<Bishop>(Position(4, 4), true);
    gameState.piecesMap[Position(8, 6)] = std::make_shared<King>(Position(8, 6), true);
    gameState.piecesMap[Position(8, 3)] = std::make_shared<Queen>(Position(8, 3), true);


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
    gameState.isWhiteToMove = false;
    gameState.piecesMap[Position(6, 6)] = std::make_shared<King>(Position(6, 6), false);
    gameState.piecesMap[Position(6, 5)] = std::make_shared<Bishop>(Position(6, 5), false);
    gameState.piecesMap[Position(6, 4)] = std::make_shared<Queen>(Position(6, 4), true);


    LegalMovesGenerator legalMovesGenerator;
    std::vector<Move> legalMoves = legalMovesGenerator.generateLegalMoves(gameState);
    EXPECT_EQ(legalMoves.size(), 5);
    EXPECT_FALSE(moveListContainsMove(legalMoves, {{6,5}, {5,4}, EMPTY}));
}