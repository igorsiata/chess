#include "gtest/gtest.h"
#include "game.hpp"

TEST(TestMovement, Bishop) {
    Game game;
    std::string startPosition = "8/8/5b2/8/8/2B5/3R4/8 ";
    game.load_position(startPosition);
    std::ostringstream oss;
    game.generate_position(oss);
    std::cout << oss.str();
    game.generate_possible_moves();
    std::ostringstream oss_moves;
    game.output_possible_moves(oss_moves);
    std::cout << oss_moves.str();


}

TEST(TestMovement, DiscoveredCheck) {
    Game game;
    std::string startPosition = "8/8/3q2b1/8/8/8/3R4/3KB2r";
    game.load_position(startPosition);
    std::ostringstream oss;
    game.generate_position(oss);
    std::cout << oss.str();
    game.generate_possible_moves();
    std::ostringstream oss_moves;
    game.output_possible_moves(oss_moves);
    std::cout << oss_moves.str();


}

TEST(TestMovement, TwoKings) {
    Game game;
    std::string startPosition = "8/2k5/8/2K5/8/8/8/8";
    game.load_position(startPosition);
    std::ostringstream oss;
    game.generate_position(oss);
    std::cout << oss.str();
    game.generate_possible_moves();
    std::ostringstream oss_moves;
    game.output_possible_moves(oss_moves);
    std::cout << oss_moves.str();


}

TEST(TestMovement, KnigstsAndPawns) {
    Game game;
    std::string startPosition = "8/6p1/5PN1/8/3pn3/3P4/2P5/8";
    game.load_position(startPosition);
    std::ostringstream oss;
    game.generate_position(oss);
    std::cout << oss.str();
    game.generate_possible_moves();
    std::ostringstream oss_moves;
    game.output_possible_moves(oss_moves);
    std::cout << oss_moves.str();


}

