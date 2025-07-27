#include "game_logic/game.hpp"

Game::Game(const std::string &positionFEN,
           std::unique_ptr<IPlayer> whitePlayer,
           std::unique_ptr<IPlayer> blackPlayer) {
  m_boardManager.loadBoardFromFEN(positionFEN);
  m_whitePlayer = std::move(whitePlayer);
  m_blackPlayer = std::move(blackPlayer);
  m_gameStatus = GameStatus::IN_PROGRESS;
  m_playerToMove = Color::WHITE;
}

std::vector<std::pair<Position64, PieceChar>> Game::getPiecesVec() {
  std::vector<std::pair<Position64, PieceChar>> piecesVec;
  for (int pieceType = 0; pieceType < PieceType::OFF_BOARD; pieceType++) {
    const SinglePieceList &piecePositions = m_boardManager.getBoard().piecesList.getPiecesList(PieceType(pieceType));
    for (const auto &piecePos : piecePositions) {
      if(piecePos != NO_SQUARE)
      piecesVec.emplace_back(BoardHelper::pos120to64(piecePos), pieceToCharMap[pieceType]);
    }
  }
  return piecesVec;
}

void Game::executeTurn() {
  bool turnEnded = false;
  if (m_playerToMove == WHITE) {
    turnEnded = m_whitePlayer->makeMove(m_boardManager);
  }
  if (m_playerToMove == BLACK) {
    turnEnded = m_blackPlayer->makeMove(m_boardManager);
  }
  if (turnEnded) {
    m_playerToMove = m_playerToMove == WHITE ? BLACK : WHITE;
    updateGameStatus();
  }
}

void Game::updateGameStatus() {
  m_gameStatus = m_boardManager.getGameStatus();
}

const std::array<PieceChar, 12> Game::pieceToCharMap = {'P', 'N', 'B', 'R', 'Q', 'K',
                                                        'p', 'n', 'b', 'r', 'q', 'k'};