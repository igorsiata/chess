#include <gtest/gtest.h>
#include "pieces/piece.hpp"

TEST(TestPiece, TestConstructor)
{
    Position position(1, 1);
    bool isWhite = true;
    Piece piece(position, isWhite);

    ASSERT_TRUE(piece.isWhite());
    ASSERT_EQ(piece.getPosition(), position);
}

TEST(TestPiece, TestMove)
{
    Position position(1, 1);
    bool isWhite = true;
    Piece piece(position, isWhite);
    Position endPosition(2, 3);
    piece.move(endPosition);

    ASSERT_EQ(piece.getPosition(), endPosition);
}