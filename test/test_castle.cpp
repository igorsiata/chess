#include "gtest/gtest.h"
#include "game.hpp"

TEST(TestCastle, AllyPieceInWay) {
    Game game;
    std::string startPosition = "8/8/8/8/8/8/8/R3KB1R ";
    game.load_position(startPosition);
    game.generate_possible_moves();
    std::vector<Position> kingMoves = game.get_piece_moves_end_position(
            Position(5,8));
    EXPECT_NE(std::find(kingMoves.begin(), kingMoves.end(),
                        Position(3,8)), kingMoves.end());
    EXPECT_EQ(std::find(kingMoves.begin(), kingMoves.end(),
                        Position(7,8)), kingMoves.end());
    /*
    std::ostringstream oss;
    game.generate_position(oss);
    std::cout << oss.str();

    std::ostringstream oss_moves;
    game.output_possible_moves(oss_moves);
    std::cout << oss_moves.str();
    */

}

TEST(TestCastle, EnemyPieceAttacking) {
    Game game;
    std::string startPosition = "8/8/8/8/2r5/8/8/R3K2R";
    game.load_position(startPosition);
    game.generate_possible_moves();
    std::vector<Position> kingMoves = game.get_piece_moves_end_position(
            Position(5,8));
    EXPECT_EQ(std::find(kingMoves.begin(), kingMoves.end(),
                        Position(3,8)), kingMoves.end());
    EXPECT_NE(std::find(kingMoves.begin(), kingMoves.end(),
                        Position(7,8)), kingMoves.end());


}

TEST(TestCastle, KingChecked) {
    Game game;
    std::string startPosition = "8/8/8/8/1b6/8/8/R3K2R";
    game.load_position(startPosition);
    game.generate_possible_moves();
    std::vector<Position> kingMoves = game.get_piece_moves_end_position(
            Position(5,8));
    EXPECT_EQ(std::find(kingMoves.begin(), kingMoves.end(),
                        Position(3,8)), kingMoves.end());
    EXPECT_EQ(std::find(kingMoves.begin(), kingMoves.end(),
                        Position(7,8)), kingMoves.end());


}
