#include <gtest/gtest.h>
#include "game_logic/king.hpp"
#include "helper_test_functions.hpp"

TEST(TestKing, MovesPositions)
{
    King king({3, 2}, true);
    PiecesMap allPieces;
    allPieces[king.getPosition()] = std::make_shared<Piece>(king);
    king.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = king.getPossibleMoves();
    Move move1 = {{3, 1}, EMPTY};
    Move move2 = {{3, 3}, EMPTY};
    Move move3 = {{4, 2}, EMPTY};
    Move move4 = {{2, 2}, EMPTY};
    Move move5 = {{4, 1}, EMPTY};
    Move move6 = {{2, 1}, EMPTY};
    Move move7 = {{4, 3}, EMPTY};
    Move move8 = {{2, 3}, EMPTY};

    ASSERT_TRUE(contains(possibleMoves, move1));
    ASSERT_TRUE(contains(possibleMoves, move2));
    ASSERT_TRUE(contains(possibleMoves, move3));
    ASSERT_TRUE(contains(possibleMoves, move4));
    ASSERT_TRUE(contains(possibleMoves, move5));
    ASSERT_TRUE(contains(possibleMoves, move6));
    ASSERT_TRUE(contains(possibleMoves, move7));
    ASSERT_TRUE(contains(possibleMoves, move8));
}

TEST(TestKing, MovesCount)
{
    King king({4, 4}, true);
    PiecesMap allPieces;
    allPieces[king.getPosition()] = std::make_shared<Piece>(king);
    king.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = king.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 8);
}

TEST(TestKing, AllyBlocking)
{
    King king({4, 4}, true);
    King allyPiece({4, 3}, true);
    King allyPiece2({3, 3}, true);
    PiecesMap allPieces;
    allPieces[king.getPosition()] = std::make_shared<Piece>(king);
    allPieces[allyPiece.getPosition()] = std::make_shared<Piece>(allyPiece);
    allPieces[allyPiece2.getPosition()] = std::make_shared<Piece>(allyPiece2);
    king.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = king.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 6);
}

TEST(TestKing, EnemyCaptrue)
{
    King king({4, 4}, false);
    King enemyPiece({4, 3}, true);
    King enemyPiece2({3, 3}, true);
    PiecesMap allPieces;
    allPieces[king.getPosition()] = std::make_shared<Piece>(king);
    allPieces[enemyPiece.getPosition()] = std::make_shared<Piece>(enemyPiece);
    allPieces[enemyPiece2.getPosition()] = std::make_shared<Piece>(enemyPiece2);
    king.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = king.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 8);
    ASSERT_TRUE(contains(possibleMoves, Move{{3, 3}, CAPTURE}));
    ASSERT_TRUE(contains(possibleMoves, Move{{4, 3}, CAPTURE}));
}
