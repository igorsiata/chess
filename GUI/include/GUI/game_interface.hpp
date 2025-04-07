#pragma once

#include "game_logic/game.hpp"
#include <typeinfo>
#include <iostream>

class GameInterface
{
public:
    GameInterface();
    std::map<Position, char> getPiecesAsChar() const;

private:
    Game m_game;
};
