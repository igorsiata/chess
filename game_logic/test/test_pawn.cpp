#include <gtest/gtest.h>
#include "pieces/pawn.hpp"
#include "helper_test_functions.hpp"


TEST(TestPawn, MoveForward)
{
    Position position(1, 7);
    bool isWhite = true;
    Pawn pawn(position, isWhite);
    PiecesMap allPieces;
    allPieces[position] = std::make_shared<Piece>(pawn);
    pawn.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = pawn.getPossibleMoves();
    Position currentPosition = pawn.getPosition();
    Move moveOneSquare(currentPosition, Position(1, 6), EMPTY);
    Move moveTwoSquares(currentPosition, Position(1, 5), EMPTY);

    ASSERT_TRUE(moveListContainsMove(possibleMoves, moveOneSquare));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, moveTwoSquares));
    ASSERT_EQ(possibleMoves.size(), 2);
}

TEST(TestPawn, Capture)
{

    Pawn pawn({1, 7}, true);
    Pawn enemyPawn({2, 6}, false);
    PiecesMap allPieces;
    allPieces[pawn.getPosition()] = std::make_shared<Piece>(pawn);
    allPieces[enemyPawn.getPosition()] = std::make_shared<Piece>(enemyPawn);
    pawn.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = pawn.getPossibleMoves();
    Move capture(pawn.getPosition(), Position(2, 6), CAPTURE);

    ASSERT_TRUE(moveListContainsMove(pawn.getPossibleMoves(), capture));
    ASSERT_EQ(possibleMoves.size(), 3);
}

TEST(TestPawn, Blocked){
    Pawn pawn({1, 7}, true);
    Pawn enemyPawn({1, 6}, false);
    PiecesMap allPieces;
    allPieces[pawn.getPosition()] = std::make_shared<Piece>(pawn);
    allPieces[enemyPawn.getPosition()] = std::make_shared<Piece>(enemyPawn);
    pawn.findPossibleMoves(allPieces);

    ASSERT_TRUE(pawn.getPossibleMoves().empty());
}

TEST(TestPawn, BlackPawnMove){
    Pawn pawn({4, 2}, false);
    Pawn enemyPawn({3, 1}, true);
    PiecesMap allPieces;
    allPieces[pawn.getPosition()] = std::make_shared<Piece>(pawn);
    allPieces[enemyPawn.getPosition()] = std::make_shared<Piece>(enemyPawn);
    pawn.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = pawn.getPossibleMoves();
    Move moveOneSquare(pawn.getPosition(), Position(4, 3), EMPTY);
    Move moveTwoSquares(pawn.getPosition(), Position(4, 4), EMPTY);

    ASSERT_TRUE(moveListContainsMove(possibleMoves, moveOneSquare));
    ASSERT_TRUE(moveListContainsMove(possibleMoves, moveTwoSquares));
    ASSERT_EQ(possibleMoves.size(), 2);
}