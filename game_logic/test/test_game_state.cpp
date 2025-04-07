#include <gtest/gtest.h>
#include "game_logic/game_state.hpp"
#include "helper_test_functions.hpp"
#include "pieces/queen.hpp"

TEST(TestGameState, MakeMoveCapture)
{
    GameState gameState;
    PiecesMap piecesMap;
    Queen queenWhite({4, 4}, true);
    Queen queenBlack({5, 5}, false);
    piecesMap[queenWhite.getPosition()] = std::make_shared<Piece>(queenWhite);
    piecesMap[queenBlack.getPosition()] = std::make_shared<Piece>(queenBlack);
    gameState.setAllPieces(piecesMap);
    Move move({4,4}, queenBlack.getPosition(), CAPTURE);
    auto queenWhitePtr = gameState.getPiecesMap()[queenWhite.getPosition()];
    gameState.updateGameState(move);

    ASSERT_EQ(gameState.getPiecesMap().size(), 1);
    ASSERT_EQ(gameState.getPiecesMap()[queenBlack.getPosition()], queenWhitePtr);
    ASSERT_EQ(gameState.getPiecesMap()[queenBlack.getPosition()]->getPosition(), queenBlack.getPosition());
}

TEST(TestGameState, MakeMove)
{
    GameState gameState;
    PiecesMap piecesMap;
    Queen queenWhite({4, 4}, true);
    Queen queenBlack({5, 5}, false);
    piecesMap[queenWhite.getPosition()] = std::make_shared<Piece>(queenWhite);
    piecesMap[queenBlack.getPosition()] = std::make_shared<Piece>(queenBlack);
    gameState.setAllPieces(piecesMap);
    Position endPosition = {4, 6};
    Move move({4, 4}, endPosition, EMPTY);
    auto queenWhitePtr = gameState.getPiecesMap()[queenWhite.getPosition()];
    gameState.updateGameState(move);

    ASSERT_EQ(gameState.getPiecesMap().size(), 2);
    ASSERT_EQ(gameState.getPiecesMap()[endPosition], queenWhitePtr);
    ASSERT_EQ(gameState.getPiecesMap()[endPosition]->getPosition(), endPosition);
}