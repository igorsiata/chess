#pragma once
#include "game_logic/defs.hpp"
#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <vector>

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
  return (startPos&0xFF) | ((endPos&0xFF) << 8) | ((capturedPiece&0xF) << 16) | ((promotedPiece&0xF) << 20) | ((flags&0xF) << 24);
}
inline Move addPromotionToMove(Move move, int promotion) {
  move &= ~(0xF << 20);
  move |= (promotion & 0xF) << 20;
  return move;
}
}; // namespace MoveHelper

namespace BoardHelper {
inline int pos120to64(int index) { return index - 15 - 2 * (index / 10 + 1); }
inline int pos64to120(int index) { return index + 19 + 2 * (index / 8 + 1); }
}; // namespace BoardHelper

struct PreviousMove {
  Move move;
  Position120 enPassantSquare;
  int fiftyMoveCounter;
  uint8_t castleRights;
  PosHash positionKey;
};

struct Board {
  PieceType pieces[120];
  Color sideToMove;
  Position120 enPassantSquare;
  int fiftyMoveCounter;
  uint8_t castleRights;

  PosHash positionKey;
  std::vector<PreviousMove> previousMovesVec;
  std::map<PosHash, int> positionKeysMap;
  int moveCount;

  Position120 kingsPositions[2];
  std::array<std::array<Position120, 10>, 12> piecesList;

  bool isPieceAlly(Position120 piecePos) {
    PieceType otherPiece = pieces[piecePos];
    if (sideToMove == Color::WHITE)
      return otherPiece < 6;
    else
      return otherPiece > 5 && otherPiece < 12;
  }

  bool isPieceEnemy(Position120 piecePos) {
    PieceType otherPiece = pieces[piecePos];
    if (sideToMove == Color::WHITE)
      return otherPiece > 5 && otherPiece < 12;
    else
      return otherPiece < 6;
  }

  int findPieceIdx(PieceType pieceType, Position120 startPos) {
    const auto &pieceArray = piecesList[pieceType];
    auto it = std::find(pieceArray.begin(), pieceArray.end(), startPos);
    if (it != pieceArray.end())
      return std::distance(pieceArray.begin(), it);

    std::perror("Piece not in list\n");
    return -1;
  }

  void removePieceFromList(PieceType pieceType, Position120 piecePos) {
    int pieceIdx = findPieceIdx(pieceType, piecePos);
    piecesList[pieceType][pieceIdx] = NO_SQUARE;
  }

  void addPieceToList(PieceType pieceType, Position120 piecePos) {
    auto &pieceArray = piecesList[pieceType];
    auto it = std::find(pieceArray.begin(), pieceArray.end(), NO_SQUARE);
    if (it != pieceArray.end())
      *it = piecePos;
  }
};
