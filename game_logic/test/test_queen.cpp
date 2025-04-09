#include <gtest/gtest.h>
#include "pieces/queen.hpp"
#include "helper_test_functions.hpp"

TEST(TestQueen, MovesPositions)
{
    Queen queen({3, 2}, true);
    PiecesMap allPieces;
    allPieces[queen.getPosition()] = std::make_shared<Piece>(queen);
    queen.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = queen.getPossibleMoves();
    Move move1({3, 2}, {3, 1}, EMPTY);
    Move move2({3, 2}, {3, 6}, EMPTY);
    Move move3({3, 2}, {7, 2}, EMPTY);
    Move move4({3, 2}, {1, 2}, EMPTY);
    Move move5({3, 2}, {7, 6}, EMPTY);
    Move move6({3, 2}, {2, 1}, EMPTY);
    Move move7({3, 2}, {4, 1}, EMPTY);
    Move move8({3, 2}, {2, 3}, EMPTY);

    ASSERT_TRUE(moveListContainsMove(possibleMoves, move1));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move2));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move3));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move4));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move5));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move6));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move7));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move8));
}

TEST(TestQueen, MovesCount)
{
    Queen queen({4, 4}, true);
    PiecesMap allPieces;
    allPieces[queen.getPosition()] = std::make_shared<Piece>(queen);
    queen.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = queen.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 27);
}

TEST(TestQueen, AllyBlocking)
{
    Queen queen({4, 4}, true);
    Queen allyPiece({4, 2}, true);
    Queen allyPiece2({2, 4}, true);
    PiecesMap allPieces;
    allPieces[queen.getPosition()] = std::make_shared<Piece>(queen);
    allPieces[allyPiece.getPosition()] = std::make_shared<Piece>(allyPiece);
    allPieces[allyPiece2.getPosition()] = std::make_shared<Piece>(allyPiece2);
    queen.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = queen.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 23);
}

TEST(TestQueen, EnemyCaptrue)
{
    Queen queen({4, 4}, false);
    Queen enemyPiece({4, 2}, true);
    Queen enemyPiece2({7, 4}, true);
    PiecesMap allPieces;
    allPieces[queen.getPosition()] = std::make_shared<Piece>(queen);
    allPieces[enemyPiece.getPosition()] = std::make_shared<Piece>(enemyPiece);
    allPieces[enemyPiece2.getPosition()] = std::make_shared<Piece>(enemyPiece2);
    queen.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = queen.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 25);
    ASSERT_TRUE(moveListContainsMove(possibleMoves, Move({4, 4}, {7, 4}, CAPTURE)));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, Move({4, 4}, {4, 2}, CAPTURE)));
}
