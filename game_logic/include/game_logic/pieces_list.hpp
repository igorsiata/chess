#pragma once

#include <algorithm>
#include <array>
#include <iostream>

#include "game_logic/move.hpp"

using SinglePieceList = std::array<Position120, 10>;

class PiecesList {
public:
  void addPiece(PieceType pieceType, Position120 pos120) {
    int pieceIdx = findPieceIdx(pieceType, NO_SQUARE);
    m_piecesList[pieceType][pieceIdx] = pos120;
  }
  void removePiece(PieceType pieceType, Position120 pos120) {
    int firstEmptyIdx = findPieceIdx(pieceType, pos120);
    m_piecesList[pieceType][firstEmptyIdx] = NO_SQUARE;
  }
  void changePiecePos(PieceType pieceType, Position120 startPos, Position120 endPos) {
    int pieceIdx = findPieceIdx(pieceType, startPos);
    m_piecesList[pieceType][pieceIdx] = endPos;
  }
  void resetPiecesList() {
    for (int idx = 0; idx < 12; idx++) {
      for (int idx2 = 0; idx2 < 10; idx2++) {
        m_piecesList[idx][idx2] = NO_SQUARE;
      }
    }
  }
  const SinglePieceList &getPiecesList(PieceType pieceType) const {
    return m_piecesList[pieceType];
  }

private:
  std::array<SinglePieceList, 12> m_piecesList;
  int findPieceIdx(PieceType pieceType, Position120 startPos) {
    const auto &pieceArray = m_piecesList[pieceType];
    auto it = std::find(pieceArray.begin(), pieceArray.end(), startPos);
    if (it != pieceArray.end())
      return std::distance(pieceArray.begin(), it);
    std::perror("Piece not in list\n");
    return -1;
  }
};