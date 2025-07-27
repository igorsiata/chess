#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include "game_logic/pieces_list.hpp"


namespace BoardHelper {
inline int pos120to64(int index) { return index - 15 - 2 * (index / 10 + 1); }
inline int pos64to120(int index) { return index + 19 + 2 * (index / 8 + 1); }
inline int getFile(Position120 pos) { return pos120to64(pos) % 8; }
inline int getRank(Position120 pos) { return pos120to64(pos) / 10; }
}; // namespace BoardHelper

struct PreviousMove {
  Move move;
  Position120 enPassantSquare;
  int fiftyMoveCounter;
  CastleRights castleRights;
  PosHash positionKey;
};

struct Board {
  PieceType pieces[120];
  Color sideToMove;
  Position120 enPassantSquare;
  int fiftyMoveCounter;
  CastleRights castleRights;

  PosHash positionKey;
  std::array<PreviousMove, 2048> previousMovesArr;
  //std::unordered_map<PosHash, int> positionKeysMap;
  int moveCount;

  Position120 kingsPositions[2];
  PiecesList piecesList;
};
