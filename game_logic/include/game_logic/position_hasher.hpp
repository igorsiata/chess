#pragma once
#include "game_logic/board.hpp"
#include <cstdlib>

class PositionHasher {
public:
  PositionHasher();
  void hashPosition(PosHash &posHash, const Board &board) const ;
  void hashPiece(PosHash &posHash, const PieceType pieceType, const Position120 piecePos) const {
    posHash ^= m_pieceKeys[pieceType][piecePos]; 
  }
  void hashCastle(PosHash &posHash, const CastleRights castleRights) const{ 
    posHash ^= m_castleKeys[castleRights]; 
  }
  void hashEnPassSqr(PosHash &posHash, const Position120 piecePos) const{
    posHash ^= m_pieceKeys[PieceType::EMPTY][piecePos]; 
  }
  void hashSide(PosHash &posHash) const {
    posHash ^= m_sideKey; 
  }


private:
  inline PosHash rand64() {
    return (PosHash)std::rand() << 32 | (PosHash)std::rand();
  }
  PosHash m_pieceKeys[14][120];
  PosHash m_sideKey;
  PosHash m_castleKeys[16];
};