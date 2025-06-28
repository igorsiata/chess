#pragma once
#include "game_logic/board.hpp"
#include <cstdlib>

class PositionHasher {
public:
  PositionHasher();
  PosHash hashPosition(const Board &board);

private:
  inline PosHash rand64() {
    return (PosHash)std::rand() << 32 | (PosHash)std::rand();
  }
  PosHash m_pieceKeys[14][120];
  PosHash m_sideKey;
  PosHash m_castleKeys[16];
};