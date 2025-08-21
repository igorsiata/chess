#pragma once

#include "game_logic/i_player.hpp"
#include <SFML/Graphics.hpp>
class GuiPlayer : public IPlayer {
public:
  GuiPlayer(Color side, std::map<char, sf::Texture> textures, sf::RenderWindow &window, sf::Vector2i &mouseClickedPos, unsigned int squareSize = 100)
      : m_textures(textures), m_color(side), m_window(window), m_mouseClickedPos(mouseClickedPos), m_squareSize(squareSize) {};
  Move chooseMove(BoardManager &boardManager) override;

private:
  Color m_color;
  sf::RenderWindow &m_window;
  sf::Vector2i &m_mouseClickedPos;
  MoveList m_legalMoves;
  Position64 m_clickedSquare;
  Position64 m_prevClickedSquare;
  bool m_isPromoting = false;
  Move m_promotingMove;
  const unsigned int m_squareSize;
  std::map<char, sf::Texture> m_textures;
  void drawPossibleMoves();
  void drawPromotionMenu();
  void updateClickedSquare();
};