#pragma once

#include <stdint.h>

#define MAX_GAME_MOVES 2048

typedef uint64_t Bitboard;

enum Position{
    A1 = 21, B1, C1, D1, E1, F1, G1, H1, I1, J1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2, I2, J2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3, I3, J3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4, I4, J4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5, I5, J5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6, I6, J6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7, I7, J7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, I8, J8, NO_SQUARE
};

enum CastleRights{
    WHITE_KINGSIDE, WHITE_QUEENSIDE, BLACK_KINGSIDE, BLACK_QUEENSIDE
};

enum PieceType {
  EMPTY,
  WHITE_PAWN,
  WHITE_KINGHT,
  WHITE_BISHOP,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_KINGHT,
  BLACK_BISHOP,
  BLACK_QUEEN,
  BLACK_KING
};

enum File {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  FILE_NONE
};

enum Rank {
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  R8,
  RANK_NONE
};

enum Color {
  WHITE,
  BLACK,
  NONE
};