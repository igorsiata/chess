#include "game_logic/game.hpp"

Game::Game(const std::string &positionFEN,
           std::unique_ptr<Player> whitePlayer,
           std::unique_ptr<Player> blackPlayer) {
  m_boardManager.loadBoardFromFEN(positionFEN);
  m_whitePlayer = std::move(whitePlayer);
  m_blackPlayer = std::move(blackPlayer);
  m_gameStatus = GameStatus::IN_PROGRESS;
  m_playerToMove = Color::WHITE;
}

std::map<Position64, PieceChar> Game::getPieceMap() const {
  std::map<Position64, PieceChar> piecesMap;
  for (int pieceType = 0; pieceType < PieceType::EMPTY; pieceType++) {
    const SinglePieceList &piecePositions = m_boardManager.getBoard().piecesList.getPiecesList(PieceType(pieceType));
    for (const auto &piecePos : piecePositions) {
      piecesMap[piecePos] = pieceToCharMap[pieceType];
    }
  }
  return piecesMap;
}

const std::array<PieceChar, 12> Game::pieceToCharMap = {'P', 'N', 'B', 'R', 'Q', 'K',
                                                        'p', 'n', 'b', 'r', 'q', 'k'};