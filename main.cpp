#include <iostream>
#include <filesystem>
#include "GUI/GUI.hpp"


int main() {
    GameInterface game;
    GUI gui(game, 100);
    return 0;
}

