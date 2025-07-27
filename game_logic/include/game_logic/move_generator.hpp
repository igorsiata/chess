#pragma once

#include "game_logic/board.hpp"
#include <array>
#include <iostream>
#include <set>

struct MoveList{
  std::array<Move, 256> moves;
  int count;
};


class MoveGenerator {
public:
  MoveGenerator(Board &board, MoveList &moveList);
  void generateAllMoves();
  bool isSquareAttackedByEnemy(const Position120 square);
  Board &m_board;

private:
  MoveList &m_moveList;
  bool isKingInCheck;
  std::set<Position120> pinnedPieces;
  const int KnightDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
  const int BishopDir[4] = {-9, -11, 9, 11};
  const int RookDir[4] = {-10, -1, 1, 10};
  const int KingQueenDir[8] = {-9, -11, 9, 11, -10, -1, 1, 10};
  inline int colorOffset() { return m_board.sideToMove == WHITE ? 0 : 6; }

  void getPinnedPieces();
  bool isPieceEnemy(Position120 piecePos);
  void addMove(Move move);
  void GeneratePawnsMoves();
  void GenerateAllSliderMoves();
  void GenerateKnightMoves();
  void GenerateKingMoves();
  void GenerateSliderMoves(const int slidingDir[],
                           int slidingDirSize,
                           Position120 startSquare);
  void addCastles();
  bool isPawnOnStartSq(Position120 piecePos, Color color);
  bool isCheckAfterMove(Move move);
  void promoteIfPossible(Move move);
};