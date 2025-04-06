#include <gtest/gtest.h>
#include "game_logic/pawn.hpp"
#include "helper_test_functions.hpp"


TEST(TestPawn, MoveForward)
{
    Position position{1, 7};
    bool isWhite = true;
    Pawn pawn(position, isWhite);
    PiecesMap allPieces;
    allPieces[position] = std::make_shared<Piece>(pawn);
    pawn.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = pawn.getPossibleMoves();
    Move moveOneSquare = {Position{1, 6}, EMPTY};
    Move moveTwoSquares = {Position{1, 5}, EMPTY};

    ASSERT_TRUE(contains(possibleMoves, moveOneSquare));
    ASSERT_TRUE(contains(possibleMoves, moveTwoSquares));
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
    Move capture = {Position{2, 6}, CAPTURE};

    ASSERT_TRUE(contains(pawn.getPossibleMoves(), capture));
    ASSERT_EQ(possibleMoves.size(), 3);
}

TEST(TestPawn, Blocked){
    Pawn pawn({1, 7}, true);
    Pawn enemyPawn({1, 6}, false);
    PiecesMap allPieces;
    allPieces[pawn.getPosition()] = std::make_shared<Piece>(pawn);
    allPieces[enemyPawn.getPosition()] = std::make_shared<Piece>(enemyPawn);
    pawn.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = pawn.getPossibleMoves();
    Move capture = {Position{2, 6}, CAPTURE};

    ASSERT_TRUE(possibleMoves.empty());
}

TEST(TestPawn, BlackPawnMove){
    Pawn pawn({4, 2}, false);
    Pawn enemyPawn({3, 1}, true);
    PiecesMap allPieces;
    allPieces[pawn.getPosition()] = std::make_shared<Piece>(pawn);
    allPieces[enemyPawn.getPosition()] = std::make_shared<Piece>(enemyPawn);
    pawn.findPossibleMoves(allPieces);
    std::vector<Move> possibleMoves = pawn.getPossibleMoves();
    Move moveOneSquare = {Position{4, 3}, EMPTY};
    Move moveTwoSquares = {Position{4, 4}, EMPTY};

    ASSERT_TRUE(contains(possibleMoves, moveOneSquare));
    ASSERT_TRUE(contains(possibleMoves, moveTwoSquares));
    ASSERT_EQ(possibleMoves.size(), 2);
}