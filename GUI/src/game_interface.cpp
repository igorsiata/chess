#include "GUI/game_interface.hpp"

GameInterface::GameInterface() {
  const std::string startPostitionFEN = "8/PPPPPPPP/8/7k/K7/8/pppppppp/8 w - - 0 1";
  m_boardManager.loadBoardFromFEN(startPostitionFEN);

  m_pieceToChar =
      {
          {PieceType::WHITE_PAWN, 'P'},
          {PieceType::WHITE_KINGHT, 'N'},
          {PieceType::WHITE_BISHOP, 'B'},
          {PieceType::WHITE_ROOK, 'R'},
          {PieceType::WHITE_QUEEN, 'Q'},
          {PieceType::WHITE_KING, 'K'},
          {PieceType::BLACK_PAWN, 'p'},
          {PieceType::BLACK_KINGHT, 'n'},
          {PieceType::BLACK_BISHOP, 'b'},
          {PieceType::BLACK_ROOK, 'r'},
          {PieceType::BLACK_QUEEN, 'q'},
          {PieceType::BLACK_KING, 'k'},
      };

  m_possibleMoves = m_boardManager.generateAllMoves();
}

std::map<Position64, char> GameInterface::getPiecesAsChar() {
  std::map<Position64, char> piecesAsChar;
  const PieceType* pieces = m_boardManager.getBoard().pieces;
  for (uint8_t pos120 = 0; pos120 < 120; pos120++) {
    if (pieces[pos120] != PieceType::EMPTY &&
        pieces[pos120] != PieceType::OFF_BOARD) {
      Position64 pos64 = BoardHelper::pos120to64(pos120);
      PieceType piece = pieces[pos120];
      piecesAsChar[pos64] = m_pieceToChar[piece];
    }
  }
  return piecesAsChar;
}

void GameInterface::movePiece(const Move &move) {
  m_boardManager.makeMove(move);
  m_possibleMoves = m_boardManager.generateAllMoves();
  GameStatus gameStatus = m_boardManager.getGameStatus();
  if (gameStatus == DRAW)
    std::cout << "DRAW\n";
  if (gameStatus == WHITE_WIN)
    std::cout << "WHITE WON\n";
  if (gameStatus == BLACK_WIN)
    std::cout << "BLACK WON\n";
}

void GameInterface::unmakeMove() {
  m_boardManager.unmakePreviousMove();
  m_possibleMoves = m_boardManager.generateAllMoves();
}