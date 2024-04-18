#ifndef CHESS_GUI_HPP
#define CHESS_GUI_HPP
#include <SFML/Graphics.hpp>
#include "Game.hpp"

class GUI{
public:
    using Textures = std::map<char, sf::Texture>;
    GUI(Game game);
    void draw_chessboard(float squareSize);
    void draw_pieces(float squareSize);
private:
    void generate_textures();
    sf::RenderWindow m_window;
    Game m_game;
    Textures m_textures;
};

#endif //CHESS_GUI_HPP
