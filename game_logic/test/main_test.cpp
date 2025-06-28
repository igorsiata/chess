#include "game_logic/board.hpp"
#include "game_logic/board_manager.hpp"
#include "game_logic/move_generator.hpp"
#include <gtest/gtest.h>

void printMoves(std::vector<Move> moves) {
  for (auto move : moves) {
    int startPos = MoveHelper::getStartPos(move);
    int endPos = MoveHelper::getEndPos(move);
    int capturedPiece = MoveHelper::getCapturedPiece(move);
    int flags = MoveHelper::getFlags(move);
    int promotedPiece = MoveHelper::getPromotedPiece(move);
    std::cout << "Move: " << startPos << "->" << endPos << " Captured:" << capturedPiece << " flags:" << flags << " Promoted:" << promotedPiece << std::endl;
  }
}

// TEST(TestMain, TestMoveGeneration1) {
//   BoardManager boardManager;
//   boardManager.loadBoardFromFEN("rn1q1b1r/pppp1ppp/8/1k2Pn2/3Q1b2/8/PPPP1PPP/RNB1KBNR w KQh -");
//   auto board = boardManager.m_board;
//   std::vector<Move> pawnMoves;
//   std::vector<Move> sliderMoves;
//   std::vector<Move> knightMoves;
//   std::vector<Move> kingMoves;
//   MoveGenerator::GeneratePawnsMoves(&board, &pawnMoves);
//   MoveGenerator::GenerateAllSliderMoves(&board, &sliderMoves);
//   MoveGenerator::GenerateKnightMoves(&board, &knightMoves);
//   MoveGenerator::GenerateKingMoves(&board, &kingMoves);
//   EXPECT_EQ(pawnMoves.size(), 13);
//   EXPECT_EQ(sliderMoves.size(), 18);
//   EXPECT_EQ(knightMoves.size(), 5);
//   EXPECT_EQ(kingMoves.size(), 2);
// }

// TEST(TestMain, TestMoveGeneration2) {
//   BoardManager boardManager;
//   boardManager.loadBoardFromFEN("rn1q1b1r/pp3ppp/3p4/1kp1Pn2/3Q1b2/8/PPPP1PPP/RNB1KBNR b KQh -");
//   auto board = boardManager.m_board;
//   std::vector<Move> pawnMoves;
//   std::vector<Move> sliderMoves;
//   std::vector<Move> knightMoves;
//   std::vector<Move> kingMoves;
//   MoveGenerator::GeneratePawnsMoves(&board, &pawnMoves);
//   MoveGenerator::GenerateAllSliderMoves(&board, &sliderMoves);
//   MoveGenerator::GenerateKnightMoves(&board, &knightMoves);
//   MoveGenerator::GenerateKingMoves(&board, &kingMoves);
//   EXPECT_EQ(pawnMoves.size(), 12);
//   EXPECT_EQ(sliderMoves.size(), 19);
//   EXPECT_EQ(knightMoves.size(), 9);
//   EXPECT_EQ(kingMoves.size(), 7);
// }

// TEST(TestMain, TestMoveGenerationENpassant) {
//   BoardManager boardManager;
//   boardManager.loadBoardFromFEN("rnbqkbnr/pp1p1ppp/8/2pPp3/8/8/PPP1PPPP/RNBQKBNR w KQkq c6");
//   auto board = boardManager.m_board;
//   std::vector<Move> pawnMoves;
//   std::vector<Move> sliderMoves;
//   std::vector<Move> knightMoves;
//   std::vector<Move> kingMoves;
//   MoveGenerator::GeneratePawnsMoves(&board, &pawnMoves);
//   EXPECT_EQ(pawnMoves.size(), 16);
// }

TEST(TestMain, TestKingMove) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("3k4/8/8/8/8/8/8/3K4 w -");
  MoveList moveList = boardManager.generateAllMoves();
  boardManager.makeMove(moveList.moves[1]);
  moveList = boardManager.generateAllMoves();
  boardManager.makeMove(moveList.moves[1]);
  moveList = boardManager.generateAllMoves();
}

TEST(TestMain, TestCastles) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("r3k2r/8/8/8/8/8/8/R3K2R b KQkq -");
  if (!boardManager.isBoardCorrect())
    std::perror("Board not correct\n");
  MoveList moveList = boardManager.generateAllMoves();
  boardManager.makeMove(moveList.moves[0]);
  moveList = boardManager.generateAllMoves();
  EXPECT_EQ(moveList.count, 26);
  EXPECT_EQ(moveList.count, 26);
}

TEST(TestMain, TestCastles2) {
  BoardManager boardManager;
  boardManager.loadBoardFromFEN("3rk2r/8/8/8/8/8/8/R3Kn1R w KQkq -");
  MoveList moveList = boardManager.generateAllMoves();
  EXPECT_EQ(moveList.count, 22);
}

TEST(TestMain, TestPawnPromotion) {

  BoardManager boardManager;
  boardManager.loadBoardFromFEN("k7/4P3/8/8/8/8/4p3/K7 w - -");
  MoveList moveList = boardManager.generateAllMoves();
  // printMoves(moves);
  boardManager.makeMove(moveList.moves[0]);
  boardManager.unmakePreviousMove();
  EXPECT_EQ(moveList.count, 7);
  moveList = boardManager.generateAllMoves();
  EXPECT_EQ(moveList.count, 7);
}