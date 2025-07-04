#pragma once

#include <cstdint>

#define MAX_GAME_MOVES 2048
#define NO_SQUARE 99

typedef uint64_t Bitboard;
typedef uint8_t Position64;
typedef uint64_t PosHash;
typedef uint8_t Position120;
typedef int Move;

enum CastleRights{
    WHITE_KINGSIDE=1, WHITE_QUEENSIDE=2, BLACK_KINGSIDE=4, BLACK_QUEENSIDE=8
};

enum PieceType {
  WHITE_PAWN,
  WHITE_KINGHT,
  WHITE_BISHOP,
  WHITE_ROOK,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_KINGHT,
  BLACK_BISHOP,
  BLACK_ROOK,
  BLACK_QUEEN,
  BLACK_KING,
  OFF_BOARD,
  EMPTY
};

enum Color {
  WHITE,
  BLACK,
  NONE
};


