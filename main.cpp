#include <iostream>
#include "GUI.hpp"


int main() {
    Game game;
    std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    game.load_position(startPosition);
    GUI gui(game);
    return 0;

}

