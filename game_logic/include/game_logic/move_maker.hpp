#pragma once

#include "game_logic/board.hpp"
#include "game_logic/position_hasher.hpp"

class MoveMaker {
public:
  MoveMaker(Board &board, const PositionHasher &positionHasher) : m_board(board), m_positionHasher(positionHasher){} 
  void makeMove(const Move move);
  void unmakeMove();

private:
  Board &m_board;
  const PositionHasher &m_positionHasher;
  void addPiece(const PieceType pieceType, const Position120 sqr);
  void clearPiece(const Position120 sqr);
  void movePiece(const Position120 startSqr, const Position120 endSqr);
  const int castlePermisions[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14,
  };
};