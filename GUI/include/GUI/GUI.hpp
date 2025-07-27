#ifndef CHESS_GUI_HPP
#define CHESS_GUI_HPP

#include "GUI/gui_player.hpp"
#include "game_logic/game.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stdio.h>

class GuiGame {
public:
  using Textures = std::map<char, sf::Texture>;
  GuiGame(const std::string &fen,
          unsigned int squareSize);
  void draw_chessboard();
  void draw_pieces();

private:
  sf::Vector2i m_mouseClickedPos;
  Game *m_game;
  sf::RenderWindow m_window;
  int m_squareSize;
  Textures m_textures;
  std::vector<std::pair<Position64, PieceChar>> m_allPieces;
  void generate_textures();
};

#endif // CHESS_GUI_HPP
