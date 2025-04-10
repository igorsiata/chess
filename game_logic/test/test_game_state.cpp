#include <gtest/gtest.h>
#include "game_logic/game_state.hpp"
#include "helper_test_functions.hpp"
#include "pieces/queen.hpp"

// TEST(TestGameState, MakeMoveCapture)
// {
//     GameState gameState;
//     PiecesMap piecesMap;
//     Queen queenWhite({4, 4}, true);
//     Queen queenBlack({5, 5}, false);
//     piecesMap[queenWhite.getPosition()] = std::make_shared<Piece>(queenWhite);
//     piecesMap[queenBlack.getPosition()] = std::make_shared<Piece>(queenBlack);
//     gameState.setAllPieces(piecesMap);
//     Move move({4,4}, queenBlack.getPosition(), CAPTURE);
//     auto queenWhitePtr = gameState.getPiecesMap()[queenWhite.getPosition()];
//     gameState.updatePiecesMap(move);

//     ASSERT_EQ(gameState.getPiecesMap().size(), 1);
//     ASSERT_EQ(gameState.getPiecesMap()[queenBlack.getPosition()], queenWhitePtr);
//     ASSERT_EQ(gameState.getPiecesMap()[queenBlack.getPosition()]->getPosition(), queenBlack.getPosition());
// }

// TEST(TestGameState, MakeMove)
// {
//     GameState gameState;
//     PiecesMap piecesMap;
//     Queen queenWhite({4, 4}, true);
//     Queen queenBlack({5, 5}, false);
//     piecesMap[queenWhite.getPosition()] = std::make_shared<Piece>(queenWhite);
//     piecesMap[queenBlack.getPosition()] = std::make_shared<Piece>(queenBlack);
//     gameState.setAllPieces(piecesMap);
//     Position endPosition = {4, 6};
//     Move move({4, 4}, endPosition, EMPTY);
//     auto queenWhitePtr = gameState.getPiecesMap()[queenWhite.getPosition()];
//     gameState.updatePiecesMap(move);

//     ASSERT_EQ(gameState.getPiecesMap().size(), 2);
//     ASSERT_EQ(gameState.getPiecesMap()[endPosition], queenWhitePtr);
//     ASSERT_EQ(gameState.getPiecesMap()[endPosition]->getPosition(), endPosition);
// }

TEST(TestGameState, LoadPosition)
{
    const std::string positionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kq e3";
    GameState gameState;
    gameState.loadGameStateFromFEN(positionFEN);

    ASSERT_EQ(gameState.getPiecesMap().size(), 32);
    ASSERT_TRUE(gameState.isWhiteToMove());
    EXPECT_TRUE(gameState.getAvaliableCastles()[true].castleKingside);
    EXPECT_FALSE(gameState.getAvaliableCastles()[true].castleQueenside);
    EXPECT_FALSE(gameState.getAvaliableCastles()[false].castleKingside);
    EXPECT_TRUE(gameState.getAvaliableCastles()[false].castleQueenside);
    EXPECT_TRUE(gameState.isEnPassantPossible());
    EXPECT_EQ(gameState.getEnPassantSquare(), Position(5, 6));
}

TEST(TestGameState, RemoveCastleRights)
{
    const std::string positionFEN = "r3k2r/1ppppppp/8/8/7P/8/1P1PPPP1/RB2KB1R b KQkq -";
    GameState gameState;
    gameState.loadGameStateFromFEN(positionFEN);
    gameState.movePiece({{1,1}, {1, 8}, CAPTURE});

    EXPECT_FALSE(gameState.getAvaliableCastles()[true].castleQueenside);
    EXPECT_FALSE(gameState.getAvaliableCastles()[false].castleQueenside);
    EXPECT_TRUE(gameState.getAvaliableCastles()[true].castleKingside);
    EXPECT_TRUE(gameState.getAvaliableCastles()[false].castleKingside);
}
