#include <gtest/gtest.h>
#include "pieces/king.hpp"
#include "helper_test_functions.hpp"

TEST(TestKing, MovesPositions)
{
    King king({3, 2}, true);
    PiecesMap allPieces;
    allPieces[king.getPosition()] = std::make_shared<Piece>(king);
    king.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = king.getPossibleMoves();
    Position currentPostion = king.getPosition();
    Move move1(currentPostion, {3, 1}, EMPTY);
    Move move2(currentPostion, {3, 3}, EMPTY);
    Move move3(currentPostion, {4, 2}, EMPTY);
    Move move4(currentPostion, {2, 2}, EMPTY);
    Move move5(currentPostion, {4, 1}, EMPTY);
    Move move6(currentPostion, {2, 1}, EMPTY);
    Move move7(currentPostion, {4, 3}, EMPTY);
    Move move8(currentPostion, {2, 3}, EMPTY);

    ASSERT_TRUE(moveListContainsMove(possibleMoves, move1));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move2));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move3));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move4));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move5));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move6));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move7));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move8));
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
    ASSERT_TRUE(moveListContainsMove(possibleMoves, Move({4, 4}, {3, 3}, CAPTURE)));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, Move({4, 4}, {4, 3}, CAPTURE)));
}
