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
void PositionHasher::hashPosition(PosHash &posHash, const Board &board) const {
  posHash = 0;
  for (int piecePos = 0; piecePos < 120; piecePos++) {
    PieceType pieceType = board.pieces[piecePos];
    if (pieceType != EMPTY && pieceType != OFF_BOARD) {
      hashPiece(posHash, pieceType, piecePos);
    }
  }
  if (board.sideToMove == WHITE)
    hashSide(posHash);

  if (board.enPassantSquare != NO_SQUARE)
    hashEnPassSqr(posHash, board.enPassantSquare);

  hashCastle(posHash, board.castleRights);
}