#include "GUI/GUI.hpp"
#include "GUI/gui_player.hpp"

int main() {

    const std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    GuiGame guiGame(fenPosition, 100);
    return 0;
}