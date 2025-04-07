#include <gtest/gtest.h>
#include "game_logic/position_loader.hpp"

TEST(TestPositionLoader, LoadPosition)
{
    const std::string positionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
    PositionLoader positionLoader;
    GameState gameState = positionLoader.loadPositionFromFEN(positionFEN);

    ASSERT_EQ(gameState.piecesMap.size(), 32);
    ASSERT_TRUE(gameState.isWhiteToMove);
}