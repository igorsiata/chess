#include <gtest/gtest.h>
#include "pieces/bishop.hpp"
#include "helper_test_functions.hpp"

TEST(TestBishop, MovesPositions)
{
    Bishop bishop({3, 2}, true);
    PiecesMap allPieces;
    allPieces[bishop.getPosition()] = std::make_shared<Piece>(bishop);
    bishop.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = bishop.getPossibleMoves();
    Move move1({3,2}, {7, 6}, EMPTY);
    Move move2({3,2}, {2, 1}, EMPTY);
    Move move3({3,2}, {4, 1}, EMPTY);
    Move move4({3,2}, {2, 3}, EMPTY);

    ASSERT_TRUE(moveListContainsMove(possibleMoves, move1));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move2));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move3));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, move4));
}

TEST(TestBishop, MovesCount)
{
    Bishop bishop({4, 4}, true);
    PiecesMap allPieces;
    allPieces[bishop.getPosition()] = std::make_shared<Piece>(bishop);
    bishop.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = bishop.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 13);
}

TEST(TestBishop, AllyBlocking)
{
    Bishop bishop({4, 4}, true);
    Bishop allyPiece({2, 2}, true);
    Bishop allyPiece2({5, 3}, true);
    PiecesMap allPieces;
    allPieces[bishop.getPosition()] = std::make_shared<Piece>(bishop);
    allPieces[allyPiece.getPosition()] = std::make_shared<Piece>(allyPiece);
    allPieces[allyPiece2.getPosition()] = std::make_shared<Piece>(allyPiece2);
    bishop.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = bishop.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 8);
}

TEST(TestBishop, EnemyCaptrue)
{
    Bishop bishop({4, 4}, false);
    Bishop enemyPiece({2, 2}, true);
    Bishop enemyPiece2({5, 3}, true);
    PiecesMap allPieces;
    allPieces[bishop.getPosition()] = std::make_shared<Piece>(bishop);
    allPieces[enemyPiece.getPosition()] = std::make_shared<Piece>(enemyPiece);
    allPieces[enemyPiece2.getPosition()] = std::make_shared<Piece>(enemyPiece2);
    bishop.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = bishop.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 10);
}
