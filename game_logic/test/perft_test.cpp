#include "game_logic/board.hpp"
#include "game_logic/board_manager.hpp"
#include "game_logic/move_generator.hpp"
#include <chrono>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <utility>

typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

template <typename F, typename... Args>
double funcTime(F func, Args &&...args) {
  TimeVar t1 = timeNow();
  func(std::forward<Args>(args)...);
  return duration(timeNow() - t1);
}

struct PerftStats {
  uint64_t nodes = 0;
  uint64_t captures = 0;
  uint64_t castles = 0;
  uint64_t enPass = 0;
  uint64_t promotions = 0;
  uint64_t checkmates = 0;
  double moveGenDuration = 0;
  double makeMoveDuration = 0;
  double unmakeMoveDuration = 0;
};

std::string moveToString(const Move move) {
  std::stringstream ss;
  ss << char('a' + BoardHelper::pos120to64(MoveHelper::getStartPos(move)) % 8) << char('8' - BoardHelper::pos120to64(MoveHelper::getStartPos(move)) / 8) << char('a' + BoardHelper::pos120to64(MoveHelper::getEndPos(move)) % 8) << char('8' - BoardHelper::pos120to64(MoveHelper::getEndPos(move)) / 8);
  if (MoveHelper::isMovePromotion(move)) {
    ss << "=" << MoveHelper::getPromotedPiece(move);
  }
  return ss.str();
}

PerftStats perftTestWithStats(BoardManager &boardManager, int maxDepth, int depth = 0) {
  PerftStats stats;

  if (depth >= maxDepth) {
    stats.nodes = 1;
    return stats;
  }
  auto now = timeNow();
  MoveList allMoves = boardManager.generateAllMoves();
  auto duration = duration(timeNow() - now);
  stats.moveGenDuration += duration;
  if (boardManager.getGameStatus() != IN_PROGRESS) {
    if (boardManager.getGameStatus() != DRAW)
      stats.checkmates = 1;
    stats.nodes = 1;
    return stats;
  }
  // printMoves(allMoves);
  for (int moveIdx = 0; moveIdx < allMoves.count; moveIdx++) {
    Move move = allMoves.moves[moveIdx];
    bool isCaptured = MoveHelper::getCapturedPiece(move) != EMPTY;
    bool isCastle = MoveHelper::isMoveCastle(move);
    bool isPromotion = MoveHelper::isMovePromotion(move);
    bool isEnPass = MoveHelper::isMoveEnPassant(move);

    now = timeNow();
    boardManager.makeMove(move);
    duration = duration(timeNow() - now);
    stats.makeMoveDuration += duration;
    // printMoves(allMoves);
    if (!boardManager.isBoardCorrect())
      std::cout << "Board not correct\n";
    PerftStats childStats = perftTestWithStats(boardManager, maxDepth, depth + 1);
    now = timeNow();
    boardManager.unmakePreviousMove();
    duration = duration(timeNow() - now);
    stats.unmakeMoveDuration += duration;
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
    stats.checkmates += childStats.checkmates;

    if (depth == 0) {
      std::cout << moveToString(move) << ": " << childStats.nodes << std::endl;
    }
  }

  return stats;
}

uint64_t perftTest(BoardManager &boardManager, int maxDepth, int depth = 0) {

  if (depth >= maxDepth)
    return 1;

  MoveList allMoves = boardManager.generateAllMoves();
  if (boardManager.getGameStatus() != IN_PROGRESS)
    return 1;
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
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 8902);
}

TEST(TestPerft, TestPos2depth1) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  PerftStats preftStats = perftTestWithStats(boardManager, 1);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 48);
  EXPECT_EQ(preftStats.captures, 8);
  EXPECT_EQ(preftStats.castles, 2);
  EXPECT_EQ(preftStats.enPass, 0);
  EXPECT_EQ(preftStats.promotions, 0);
}

TEST(TestPerft, TestPos2depth2) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  PerftStats preftStats = perftTestWithStats(boardManager, 2);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 2039);
  EXPECT_EQ(preftStats.captures, 351 + 8);
  EXPECT_EQ(preftStats.castles, 91 + 2);
  EXPECT_EQ(preftStats.enPass, 1);
  EXPECT_EQ(preftStats.promotions, 0);
}

TEST(TestPerft, TestPos2depth3) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  PerftStats preftStats = perftTestWithStats(boardManager, 3);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
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
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
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
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 9467);
  EXPECT_EQ(preftStats.captures, 1021 + 87);
  EXPECT_EQ(preftStats.castles, 0 + 6);
  EXPECT_EQ(preftStats.enPass, 4);
  EXPECT_EQ(preftStats.promotions, 120 + 48);
}

TEST(TestPerft, TestPos2depth4) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
  PerftStats preftStats = perftTestWithStats(boardManager, 4);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 4085603);
  EXPECT_EQ(preftStats.captures, 8 + 351 + 17102 + 757163);
  EXPECT_EQ(preftStats.castles, 2 + 91 + 3162 + 128013);
  EXPECT_EQ(preftStats.enPass, 1 + 45 + 1929);
  EXPECT_EQ(preftStats.promotions, 15172);
  EXPECT_EQ(preftStats.checkmates, 1);
}

TEST(TestPerft, TestPos4depth4) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  PerftStats preftStats = perftTestWithStats(boardManager, 4);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 422333);
  EXPECT_EQ(preftStats.captures, 131393 + 1021 + 87);
  EXPECT_EQ(preftStats.castles, 0 + 6 + 7795);
  EXPECT_EQ(preftStats.enPass, 4);
  EXPECT_EQ(preftStats.promotions, 120 + 48 + 60032);
  EXPECT_EQ(preftStats.checkmates, 22);
}

TEST(TestPerft, TestPos5depth3) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  PerftStats preftStats = perftTestWithStats(boardManager, 3);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 62379);
}
TEST(TestPerft, TestPos5depth4) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  std::cout << "-----\n";
  PerftStats preftStats = perftTestWithStats(boardManager, 4);
  // std::cout << "MoveGen: " << preftStats.moveGenDuration << "\nMoveMake: " << preftStats.makeMoveDuration << "\nUnmakeMove: " << preftStats.unmakeMoveDuration << std::endl;
  EXPECT_EQ(preftStats.nodes, 2103487);
}