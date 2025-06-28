#ifndef CHESS_GUI_HPP
#define CHESS_GUI_HPP

#include "GUI/game_interface.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>

struct Position {
  Position(float x_, float y_) : x(x_), y(y_) {}
  Position(Position120 pos120) {
    Position64 pos64 = BoardHelper::pos120to64(pos120);
    x = pos64 % 8;
    y = (int)(pos64 / 8);
  }
  Position64 posGUIto64() const {
    return Position64(((int)y) * 8 + ((int)x));
  }
  Position120 posGUIto120() const {
    return BoardHelper::pos64to120(posGUIto64());
  }
  float x;
  float y;
};

class GUI {
public:
  using Textures = std::map<char, sf::Texture>;

  GUI();

  void draw_chessboard();

  void draw_pieces();

  void draw_possible_moves();

  void left_mouse_button_clicked();
  void drawPromotionMenu();
  void promotePiece();

private:
  sf::RenderWindow m_window;
  int m_squareSize;
  Position m_clickedSquare;
  std::vector<Move> m_clickedPieceMoves;
  GameInterface m_gameInterface;
  Textures m_textures;
  bool m_isPromoting;
  Move m_promotingMove;

  Position get_mouse_position();

  void generate_textures();
  void movePiece(const Position &startSquare, const Position &endSquare);
};

#endif // CHESS_GUI_HPP
