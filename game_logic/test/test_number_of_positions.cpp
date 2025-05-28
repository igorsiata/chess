#include <gtest/gtest.h>
#include "game_logic/game.hpp"

uint32_t countPossiblePossitions(Game &game, uint8_t max_depth, uint32_t counter = 0, uint8_t depth = 0)
{
    if (depth >= max_depth)
        return 0;
    std::vector<Move> allMoves = game.getPossibleMoves();
    for (const auto &move : allMoves)
    {
        game.makeMove(move);
        countPossiblePossitions(game, max_depth, counter, depth + 1);
        game.unmakeMove(move);
    }
    return 1;
}

TEST(TestNumberOfPositions, Depth1)
{
    Game game;
    std::string positionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
    game.loadPositionFEN(positionFEN);
    uint32_t counter = countPossiblePossitions(game, 3);
    EXPECT_EQ(counter, 40);
}