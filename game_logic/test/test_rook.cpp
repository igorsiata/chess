#include <gtest/gtest.h>
#include "game_logic/rook.hpp"
#include "helper_test_functions.hpp"

TEST(TestRook, MovesPositions)
{
    Rook rook({3, 2}, true);
    PiecesMap allPieces;
    allPieces[rook.getPosition()] = std::make_shared<Piece>(rook);
    rook.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = rook.getPossibleMoves();
    Move move1 = {{3, 1}, EMPTY};
    Move move2 = {{3, 6}, EMPTY};
    Move move3 = {{7, 2}, EMPTY};
    Move move4 = {{1, 2}, EMPTY};

    ASSERT_TRUE(contains(possibleMoves, move1));
    ASSERT_TRUE(contains(possibleMoves, move2));
    ASSERT_TRUE(contains(possibleMoves, move3));
    ASSERT_TRUE(contains(possibleMoves, move4));
}

TEST(TestRook, MovesCount)
{
    Rook rook({4, 4}, true);
    PiecesMap allPieces;
    allPieces[rook.getPosition()] = std::make_shared<Piece>(rook);
    rook.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = rook.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 14);
}

TEST(TestRook, AllyBlocking)
{
    Rook rook({4, 4}, true);
    Rook allyPiece({4, 2}, true);
    Rook allyPiece2({2, 4}, true);
    PiecesMap allPieces;
    allPieces[rook.getPosition()] = std::make_shared<Piece>(rook);
    allPieces[allyPiece.getPosition()] = std::make_shared<Piece>(allyPiece);
    allPieces[allyPiece2.getPosition()] = std::make_shared<Piece>(allyPiece2);
    rook.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = rook.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 10);
}

TEST(TestRook, EnemyCaptrue)
{
    Rook rook({4, 4}, false);
    Rook enemyPiece({4, 2}, true);
    Rook enemyPiece2({7, 4}, true);
    PiecesMap allPieces;
    allPieces[rook.getPosition()] = std::make_shared<Piece>(rook);
    allPieces[enemyPiece.getPosition()] = std::make_shared<Piece>(enemyPiece);
    allPieces[enemyPiece2.getPosition()] = std::make_shared<Piece>(enemyPiece2);
    rook.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = rook.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 12);
    ASSERT_TRUE(contains(possibleMoves, Move{{7, 4}, CAPTURE}));
    ASSERT_TRUE(contains(possibleMoves, Move{{4, 2}, CAPTURE}));
}
