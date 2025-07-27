#pragma once

#include "game_logic/board_manager.hpp"
#include "game_logic/move_generator.hpp"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <typeinfo>

class GameInterface {
public:
  GameInterface();
  std::map<Position64, char> getPiecesAsChar();
  void movePiece(const Move &move);
  const MoveList &getPossibleMoves() const { return m_possibleMoves; }
  bool isWhiteToMove() const { return m_boardManager.isWhiteToMove(); }
  void unmakeMove();
  int getFullMoveCount() const { return m_boardManager.getBoard().moveCount; }

private:
  BoardManager m_boardManager;
  MoveList m_possibleMoves;
  std::map<PieceType, char> m_pieceToChar;
};
