#include "GUI/GUI.hpp"

GuiGame::GuiGame(const std::string &fen,
                 unsigned int squareSize = 100)
    : m_squareSize(squareSize) {

  m_window.create({squareSize * 8, squareSize * 8}, "Chess");
  generate_textures();
  auto whitePlayer = std::make_unique<GuiPlayer>(WHITE, m_textures, m_window, m_mouseClickedPos, squareSize);
  auto blackPlayer = std::make_unique<GuiPlayer>(BLACK, m_textures, m_window, m_mouseClickedPos, squareSize);
  m_game = new Game(fen, std::move(whitePlayer), std::move(blackPlayer));
  m_window.setFramerateLimit(60);

  while (m_window.isOpen()) {
    for (auto event = sf::Event{}; m_window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        m_window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) // Use event data
        m_mouseClickedPos = sf::Mouse::getPosition(m_window);
    }
    m_window.clear();
    draw_chessboard();
    draw_pieces();
    if (m_game->getGameStatus() == IN_PROGRESS) {
      m_game->executeTurn();
    }
    m_window.display();
  }
}

void GuiGame::generate_textures() {
  // white pieces
  m_textures['K'] = sf::Texture();
  if (!m_textures['K'].loadFromFile("textures/white_king.png"))
    return;
  m_textures['Q'] = sf::Texture();
  if (!m_textures['Q'].loadFromFile("textures/white_queen.png"))
    return;
  m_textures['B'] = sf::Texture();
  if (!m_textures['B'].loadFromFile("textures/white_bishop.png"))
    return;
  m_textures['N'] = sf::Texture();
  if (!m_textures['N'].loadFromFile("textures/white_kinght.png"))
    return;
  m_textures['R'] = sf::Texture();
  if (!m_textures['R'].loadFromFile("textures/white_rook.png"))
    return;
  m_textures['P'] = sf::Texture();
  if (!m_textures['P'].loadFromFile("textures/white_pawn.png"))
    return;

  // black pieces
  m_textures['k'] = sf::Texture();
  if (!m_textures['k'].loadFromFile("textures/black_king.png"))
    return;
  m_textures['q'] = sf::Texture();
  if (!m_textures['q'].loadFromFile("textures/black_queen.png"))
    return;
  m_textures['b'] = sf::Texture();
  if (!m_textures['b'].loadFromFile("textures/black_bishop.png"))
    return;
  m_textures['n'] = sf::Texture();
  if (!m_textures['n'].loadFromFile("textures/black_kinght.png"))
    return;
  m_textures['r'] = sf::Texture();
  if (!m_textures['r'].loadFromFile("textures/black_rook.png"))
    return;
  m_textures['p'] = sf::Texture();
  if (!m_textures['p'].loadFromFile("textures/black_pawn.png"))
    return;
}

void GuiGame::draw_chessboard() {
  m_window.RenderTarget::clear(sf::Color(169, 169, 169));
  for (std::size_t y = 0; y <= 7; y++) {
    for (std::size_t x = 0; x <= 3; x++) {
      sf::RectangleShape square;
      square.setSize(sf::Vector2f(m_squareSize, m_squareSize));
      square.setFillColor(sf::Color(249, 246, 238));
      square.setPosition(((x * 2 * m_squareSize) + m_squareSize * (y % 2)),
                         (y * m_squareSize));
      m_window.draw(square);
    }
  }
}

void GuiGame::draw_pieces() {

  m_allPieces = m_game->getPiecesVec();
  for (const auto &[position, pieceChar] : m_allPieces) {
    int xPos = (position % 8) * m_squareSize;
    int yPos = (position / 8) * m_squareSize;
    sf::RectangleShape piece;
    piece.setSize(sf::Vector2f(m_squareSize, m_squareSize));
    piece.setTexture(&m_textures[pieceChar]);
    piece.setPosition(xPos, yPos);
    m_window.draw(piece);
  }
}

// Position GUI::get_mouse_position() {
//   sf::Vector2i position = sf::Mouse::getPosition(m_window);
//   Position pos(((int)position.x / 100), ((int)position.y / 100));
//   return pos;
// }

// void GUI::draw_possible_moves() {
//   Position64 piecePos120 = m_clickedSquare.posGUIto120();
//   const MoveList possibleMoves = m_gameInterface.getPossibleMoves();
//   for (int moveIdx = 0; moveIdx < possibleMoves.count; moveIdx++) {
//     Move move = possibleMoves.moves[moveIdx];
//     if (MoveHelper::getStartPos(move) != piecePos120)
//       continue;
//     Position64 endPos = BoardHelper::pos120to64(MoveHelper::getEndPos(move));
//     sf::RectangleShape square;
//     square.setSize(sf::Vector2f(m_squareSize, m_squareSize));
//     square.setFillColor(sf::Color(201, 77, 68, 128));
//     square.setPosition((endPos % 8) * m_squareSize,
//                        (endPos / 8) * m_squareSize);
//     m_window.draw(square);
//   }
// }

// void GUI::left_mouse_button_clicked() {
//   Position newClickedSquare = get_mouse_position();
//   if (m_isPromoting)
//     promotePiece();
//   else
//     movePiece(m_clickedSquare, newClickedSquare);
//   m_clickedSquare = newClickedSquare;
// }

// void GUI::movePiece(const Position &startSquare, const Position &endSquare) {
//   Position120 startSqr120 = startSquare.posGUIto120();
//   Position120 endSqr120 = endSquare.posGUIto120();
//   const MoveList possibleMoves = m_gameInterface.getPossibleMoves();
//   for (int moveIdx = 0; moveIdx < possibleMoves.count; moveIdx++) {
//     Move move = possibleMoves.moves[moveIdx];
//     if (MoveHelper::getStartPos(move) != startSqr120 || MoveHelper::getEndPos(move) != endSqr120)
//       continue;
//     if (MoveHelper::isMovePromotion(move)) {
//       m_isPromoting = true;
//       m_promotingMove = move;
//       break;

//     } else {
//       m_promotingMove = move;
//       m_gameInterface.movePiece(move);
//       break;
//     }
//   }
// }

// void GUI::drawPromotionMenu() {
//   Position promotingSqr(MoveHelper::getEndPos(m_promotingMove));
//   int boxXPos = promotingSqr.x * 100 - 50;
//   boxXPos = std::min(600, std::max(0, boxXPos));
//   int boxYPos = promotingSqr.y * 100 + 25;
//   sf::RectangleShape bg(sf::Vector2f(200, 50));
//   bg.setFillColor(sf::Color(0, 0, 255, 100));
//   bg.setPosition(boxXPos, boxYPos);
//   m_window.draw(bg);

//   std::array<char, 4> options = m_gameInterface.isWhiteToMove() ? std::array<char, 4>{'N', 'B', 'R', 'Q'}
//                                                                 : std::array<char, 4>{'n', 'b', 'r', 'q'};
//   for (size_t i = 0; i < options.size(); ++i) {
//     sf::RectangleShape piece;
//     piece.setSize(sf::Vector2f(m_squareSize / 2, m_squareSize / 2));
//     piece.setTexture(&m_textures[options[i]]);
//     piece.setPosition(boxXPos + 50 * i,
//                       boxYPos);
//     m_window.draw(piece);
//   }
//   // sf::Vector2i position = sf::Mouse::getPosition(m_window);
//   // std::cout<<"X: "<<position.x<<", Y: "<<position.y<<std::endl;
// }

// void GUI::promotePiece() {
//   Position promotingSqr(MoveHelper::getEndPos(m_promotingMove));
//   int boxXPos = promotingSqr.x * 100 - 50;
//   boxXPos = std::min(600, std::max(0, boxXPos));
//   int boxYPos = promotingSqr.y * 100 + 25;
//   sf::Vector2i position = sf::Mouse::getPosition(m_window);
//   int selectedPiece = (position.x - boxXPos) / 50 + 1;
//   bool isYcorrect = std::abs(position.y - boxYPos - 25) < 25;
//   bool isXcorrect = selectedPiece < 5 && position.x - boxXPos > 0;
//   if (isYcorrect && isXcorrect) {
//     if (!m_gameInterface.isWhiteToMove())
//       selectedPiece += 6;

//     m_promotingMove = MoveHelper::addPromotionToMove(m_promotingMove, selectedPiece);
//     std::cout << "Selected: " << selectedPiece << "\n";
//     std::cout << "Promoting: " << MoveHelper::getPromotedPiece(m_promotingMove) << "\n";

//     m_gameInterface.movePiece(m_promotingMove);
//     m_isPromoting = false;
//   }
// }
