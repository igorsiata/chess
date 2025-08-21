#pragma once
#include "game_logic/defs.hpp"

#define MOVE_FLAG_ENPASSANT 4
#define MOVE_FLAG_CASTLE 2
#define MOVE_FLAG_PAWNSTART 1
#define MOVE_FLAG_REGULAR_MOVE 0
#define MOVE_FLAG_NO_PROMOTION 0
#define MOVE_FLAG_ANY_PROMOTION 0x78

class Move {
public:
  Move() : m_data(0) {}
  constexpr Move(int startPos, int endPos, int capturedPiece, int promotedPiece, int flags) noexcept
      : m_data((startPos & 0xFF) |
               ((endPos & 0xFF) << 8) |
               ((capturedPiece & 0xF) << 16) |
               ((promotedPiece & 0xF) << 20) |
               ((flags & 0xF) << 24)) {}
  [[nodiscard]] constexpr operator bool() const noexcept { return m_data == 0 ;}
  [[nodiscard]] constexpr int startPos() const noexcept { return (m_data & 0x000000FF); }
  [[nodiscard]] constexpr int endPos() const noexcept { return (m_data & 0x0000FF00) >> 8; }
  [[nodiscard]] constexpr int capturedPiece() const noexcept { return (m_data & 0x000F0000) >> 16; }
  [[nodiscard]] constexpr int promotedPiece() const noexcept { return (m_data & 0x00F00000) >> 20; }
  [[nodiscard]] constexpr int flags() const noexcept { return (m_data >> 24) & 0xF; }
  [[nodiscard]] constexpr bool isPawnStart() const noexcept { return (m_data & (1 << 24)) != 0; }
  [[nodiscard]] constexpr bool isCapture() const noexcept { return capturedPiece() != PieceType::EMPTY; }
  [[nodiscard]] constexpr bool isCastle() const noexcept { return (m_data & (1 << 25)) != 0; }
  [[nodiscard]] constexpr bool isEnPassant() const noexcept { return (m_data & (1 << 26)) != 0; }
  [[nodiscard]] constexpr bool isPromotion() const noexcept { return (m_data & 0x00F00000) != 0; }
  void addPromotion(int promotion) noexcept {
    m_data &= ~(0xF << 20);
    m_data |= (promotion & 0xF) << 20;
  }

private:
  //      flag prom cap  endpos    startpos
  // 0000 0000 0000 0000 0000 0000 0000 0000
  uint32_t m_data;
};
