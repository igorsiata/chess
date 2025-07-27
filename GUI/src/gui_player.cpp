#include "GUI/gui_player.hpp"

void GuiPlayer::updateClickedSquare() {
  Position64 clickedSqr = (m_mouseClickedPos.y / m_squareSize * 8) + (m_mouseClickedPos.x / m_squareSize);
  if (clickedSqr != m_clickedSquare) {
    m_prevClickedSquare = m_clickedSquare;
    m_clickedSquare = clickedSqr;
  }
}

void GuiPlayer::drawPossibleMoves() {
  const Position120 mousePos120 = BoardHelper::pos64to120(m_clickedSquare);

  for (int moveIdx = 0; moveIdx < m_legalMoves.count; moveIdx++) {
    Move move = m_legalMoves.moves[moveIdx];
    if (MoveHelper::getStartPos(move) != mousePos120)
      continue;
    Position64 endPos = BoardHelper::pos120to64(MoveHelper::getEndPos(move));
    sf::RectangleShape square;
    square.setSize(sf::Vector2f(m_squareSize, m_squareSize));
    square.setFillColor(sf::Color(201, 77, 68, 128));
    square.setPosition((endPos % 8) * m_squareSize,
                       (endPos / 8) * m_squareSize);
    m_window.draw(square);
  }
}

Move GuiPlayer::chooseMove(BoardManager &boardManager) {
  if (m_isPromoting){
    drawPromotionMenu();
  }
  m_legalMoves = boardManager.generateAllMoves();
  updateClickedSquare();
  drawPossibleMoves();
  Position120 startSquare = BoardHelper::pos64to120(m_prevClickedSquare);
  Position120 endSquare = BoardHelper::pos64to120(m_clickedSquare);
  for (int i = 0; i < m_legalMoves.count; i++) {
    Move move = m_legalMoves.moves[i];
    if (MoveHelper::getStartPos(move) == startSquare &&
        MoveHelper::getEndPos(move) == endSquare) {
      if (MoveHelper::isMovePromotion(move)) {
        m_isPromoting = true;
        m_promotingMove = move;
        break;
      } else {
        m_prevClickedSquare = NO_SQUARE;
        m_clickedSquare = NO_SQUARE;
        return move;
      }
    }
  }
  return 0;
}

void GuiPlayer::drawPromotionMenu() {
  
  int boxXPos = m_mouseClickedPos.x * 100 - 50;
  boxXPos = std::min(600, std::max(0, boxXPos));
  int boxYPos = m_mouseClickedPos.y * 100 + 25;
  sf::RectangleShape bg(sf::Vector2f(200, 50));
  bg.setFillColor(sf::Color(0, 0, 255, 100));
  bg.setPosition(boxXPos, boxYPos);
  m_window.draw(bg);

  std::array<char, 4> options = m_color == WHITE ? std::array<char, 4>{'N', 'B', 'R', 'Q'}
                                                                : std::array<char, 4>{'n', 'b', 'r', 'q'};
  for (size_t i = 0; i < options.size(); ++i) {
    sf::RectangleShape piece;
    piece.setSize(sf::Vector2f(m_squareSize / 2, m_squareSize / 2));
    piece.setTexture(&m_textures[options[i]]);
    piece.setPosition(boxXPos + 50 * i,
                      boxYPos);
    m_window.draw(piece);
  }
  // sf::Vector2i position = sf::Mouse::getPosition(m_window);
  // std::cout<<"X: "<<position.x<<", Y: "<<position.y<<std::endl;
}