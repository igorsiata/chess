#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <vector>

#include "game_logic/pieces_list.hpp"


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
  PiecesList piecesList;
};
