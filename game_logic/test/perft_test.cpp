#include "game_logic/board.hpp"
#include "game_logic/board_manager.hpp"
#include "game_logic/move_generator.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

struct PerftStats {
  uint64_t nodes = 0;
  uint64_t captures = 0;
  uint64_t castles = 0;
  uint64_t enPass = 0;
  uint64_t promotions = 0;
};

PerftStats perftTestWithStats(BoardManager &boardManager, int maxDepth, int depth = 0) {
  PerftStats stats;

  if (depth >= maxDepth || boardManager.getGameStatus() != IN_PROGRESS) {
    stats.nodes = 1;
    return stats;
  }

  MoveList allMoves = boardManager.generateAllMoves();
  // printMoves(allMoves);
  for (int moveIdx = 0; moveIdx < allMoves.count; moveIdx++) {
    Move move = allMoves.moves[moveIdx];
    bool isCaptured = MoveHelper::getCapturedPiece(move) != EMPTY;
    bool isCastle = MoveHelper::isMoveCastle(move);
    bool isPromotion = MoveHelper::isMovePromotion(move);
    bool isEnPass = MoveHelper::isMoveEnPassant(move);

    boardManager.makeMove(move);
    if (!boardManager.isBoardCorrect())
      std::cout << "Board not correct\n";
    PerftStats childStats = perftTestWithStats(boardManager, maxDepth, depth + 1);
    boardManager.unmakePreviousMove();
    if (!boardManager.isBoardCorrect())
      std::cout << "Board not correct\n";
    if (isCaptured)
      stats.captures++;
    if (isCastle)
      stats.castles++;
    if (isPromotion)
      stats.promotions++;
    if (isEnPass)
      stats.enPass++;

    stats.nodes += childStats.nodes;
    stats.captures += childStats.captures;
    stats.castles += childStats.castles;
    stats.promotions += childStats.promotions;
    stats.enPass += childStats.enPass;
  }

  return stats;
}

uint64_t perftTest(BoardManager &boardManager, int maxDepth, int depth = 0) {

  if (depth >= maxDepth || boardManager.getGameStatus() != IN_PROGRESS)
    return 1;

  MoveList allMoves = boardManager.generateAllMoves();
  uint64_t nodes = 0;
  for (int moveIdx = 0; moveIdx < allMoves.count; moveIdx++) {
    boardManager.makeMove(allMoves.moves[moveIdx]);
    nodes += perftTest(boardManager, maxDepth, depth + 1);
    boardManager.unmakePreviousMove();
  }
  return nodes;
}

TEST(TestPerft, TestStartPosDepth2) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  PerftStats preftStats = perftTestWithStats(boardManager, 3);
  EXPECT_EQ(preftStats.nodes, 8902);
}

TEST(TestPerft, TestPos2) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  PerftStats preftStats = perftTestWithStats(boardManager, 3);
  EXPECT_EQ(preftStats.nodes, 97862);
  EXPECT_EQ(preftStats.captures, 17102 + 351 + 8);
  EXPECT_EQ(preftStats.castles, 3162 + 91 + 2);
  EXPECT_EQ(preftStats.enPass, 45 + 1);
  EXPECT_EQ(preftStats.promotions, 0);
}

TEST(TestPerft, TestPos3) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  PerftStats preftStats = perftTestWithStats(boardManager, 4);
  EXPECT_EQ(preftStats.nodes, 43238);
  EXPECT_EQ(preftStats.captures, 3348 + 209 + 14 + 1);
  EXPECT_EQ(preftStats.castles, 0);
  EXPECT_EQ(preftStats.enPass, 123 + 2);
  EXPECT_EQ(preftStats.promotions, 0);
}

TEST(TestPerft, TestPos4) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  PerftStats preftStats = perftTestWithStats(boardManager, 3);
  EXPECT_EQ(preftStats.nodes, 9467);
  EXPECT_EQ(preftStats.captures, 1021 + 87);
  EXPECT_EQ(preftStats.castles, 0 + 6);
  EXPECT_EQ(preftStats.enPass, 4);
  EXPECT_EQ(preftStats.promotions, 120 + 48);
}

TEST(TestPerft, OnlyNodes) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
  uint64_t nodes = perftTest(boardManager, 4);
  EXPECT_EQ(nodes, 4085603);
}