#include "gtest/gtest.h"
#include "game.hpp"

TEST(DrawChessboardTest, EmptyChessboard) {
    Game game;
    std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    game.load_position(startPosition);
    std::ostringstream oss;
    game.generate_position(oss);
    std::cout<<oss.str();

}
