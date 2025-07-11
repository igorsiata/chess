#pragma once
#include "game_logic/defs.hpp"

#define MOVE_FLAG_ENPASSANT 4
#define MOVE_FLAG_CASTLE 2
#define MOVE_FLAG_PAWNSTART 1
#define MOVE_FLAG_REGULAR_MOVE 0
#define MOVE_FLAG_NO_PROMOTION 0
#define MOVE_FLAG_ANY_PROMOTION 0x78

namespace MoveHelper {
//      flag prom cap  endpos    startpos
// 0000 0000 0000 0000 0000 0000 0000 0000
inline int getStartPos(Move move) { return (move & 0x000000FF); }
inline int getEndPos(Move move) { return (move & 0x0000FF00) >> 8; }
inline int getCapturedPiece(Move move) { return (move & 0x000F0000) >> 16; }
inline int getPromotedPiece(Move move) { return (move & 0x00F00000) >> 20; }
inline int getFlags(Move move) { return (move >> 24) & 0xF; }
inline bool isMovePawnStart(Move move) { return (move & (1 << 24)) != 0; }
inline bool isMoveCastle(Move move) { return (move & (1 << 25)) != 0; }
inline bool isMoveEnPassant(Move move) { return (move & (1 << 26)) != 0; }
inline bool isMovePromotion(Move move) { return (move & 0x00F00000) != 0; }
inline Move createMove(int startPos, int endPos, int capturedPiece, int promotedPiece, int flags) {
  return (startPos & 0xFF) | ((endPos & 0xFF) << 8) | ((capturedPiece & 0xF) << 16) | ((promotedPiece & 0xF) << 20) | ((flags & 0xF) << 24);
}
inline Move addPromotionToMove(Move move, int promotion) {
  move &= ~(0xF << 20);
  move |= (promotion & 0xF) << 20;
  return move;
}
}; // namespace MoveHelper