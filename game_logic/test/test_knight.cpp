#include <gtest/gtest.h>
#include "game_logic/knight.hpp"
#include "helper_test_functions.hpp"



TEST(TestKnight, MovesPositions)
{
    Knight knight({1, 1}, true);
    PiecesMap allPieces;
    allPieces[knight.getPosition()] = std::make_shared<Piece>(knight);
    knight.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = knight.getPossibleMoves();
    Move move1 = {{3, 2}, EMPTY};
    Move move2 = {{2, 3}, EMPTY};

    ASSERT_TRUE(contains(possibleMoves, move1));
    ASSERT_TRUE(contains(possibleMoves, move2));
}

TEST(TestKnight, MovesCount)
{
    Knight knight({4, 4}, true);
    PiecesMap allPieces;
    allPieces[knight.getPosition()] = std::make_shared<Piece>(knight);
    knight.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = knight.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 8);
}

TEST(TestKnight, AllyBlocking)
{
    Knight knight({4, 4}, true);
    Knight allyPiece({2, 3}, true);
    PiecesMap allPieces;
    allPieces[knight.getPosition()] = std::make_shared<Piece>(knight);
    allPieces[allyPiece.getPosition()] = std::make_shared<Piece>(allyPiece);
    knight.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = knight.getPossibleMoves();

    ASSERT_EQ(possibleMoves.size(), 7);
}
