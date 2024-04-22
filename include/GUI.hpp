#ifndef CHESS_GUI_HPP
#define CHESS_GUI_HPP

#include <SFML/Graphics.hpp>
#include "Game.hpp"

class GUI {
public:
    using Textures = std::map<char, sf::Texture>;

    GUI(Game game, int squareSize);

    void draw_chessboard();

    void draw_pieces();

    void draw_possible_moves();

    void left_mouse_button_clicked();

private:


    Position get_mouse_position();

    void generate_textures();

    int m_squareSize;
    Position m_clickedSquare;
    sf::RenderWindow m_window;
    Game m_game;
    Textures m_textures;
};

#endif //CHESS_GUI_HPP
