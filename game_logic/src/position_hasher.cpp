#include "game_logic/position_hasher.hpp"

PositionHasher::PositionHasher() {
  m_sideKey = rand64();

  for (int idx1 = 0; idx1 < 13; idx1++) {
    for (int idx2 = 0; idx2 < 120; idx2++) {
      m_pieceKeys[idx1][idx2] = rand64();
    }
  }

  for (int idx1 = 0; idx1 < 16; idx1++) {
    m_castleKeys[idx1] = rand64();
  }
}
PosHash PositionHasher::hashPosition(const Board &board) {
  PosHash hash = 0;
  for (int idx = 0; idx < 120; idx++) {
    PieceType pieceType = board.pieces[idx];
    if (pieceType != EMPTY && pieceType != OFF_BOARD) {
      hash ^= m_pieceKeys[pieceType][idx];
    }
  }
  if (board.sideToMove == WHITE) {
    hash ^= m_sideKey;
  }

  if (board.enPassantSquare != NO_SQUARE) {
    hash ^= m_pieceKeys[PieceType::EMPTY][board.enPassantSquare];
  }

  hash ^= m_castleKeys[board.castleRights];
  return hash;
}