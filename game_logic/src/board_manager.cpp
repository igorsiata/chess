#include "game_logic/board_manager.hpp"

BoardManager::BoardManager() : m_moveGenerator(m_board, m_moveList), m_moveMaker(m_board, m_positionHasher) {

  resetBoard();
}

void BoardManager::loadBoardFromFEN(const std::string &positionFEN) {

  resetBoard();
  FENParser::loadPosition(positionFEN, &m_board);
  m_positionHasher.hashPosition(m_board.positionKey, m_board);
  //m_board.positionKeysMap[m_board.positionKey] += 1;
}
const MoveList BoardManager::generateAllMoves() {
  m_moveGenerator.generateAllMoves();
  m_gameStatus = updateGameStatus(m_moveList.count == 0);
  return m_moveList;
}
void BoardManager::resetBoard() {
  for (int idx = 0; idx < 120; idx++) {
    m_board.pieces[idx] = PieceType::OFF_BOARD;
  }
  m_board.piecesList.resetPiecesList();

  for (int idx = 0; idx < 64; idx++) {
    uint8_t idx120 = BoardHelper::pos64to120(idx);
    m_board.pieces[idx120] = PieceType::EMPTY;
  }

  m_board.kingsPositions[Color::WHITE] = NO_SQUARE;
  m_board.kingsPositions[Color::BLACK] = NO_SQUARE;
  m_board.sideToMove = Color::NONE;

  m_board.moveCount = 0;
  m_board.fiftyMoveCounter = 0;
  m_board.castleRights = CastleRights(0);
  m_board.positionKey = uint64_t(0);
  //m_board.positionKeysMap.clear();
}

void BoardManager::makeMove(Move move) {
  m_moveMaker.makeMove(move);
}

void BoardManager::unmakePreviousMove() {
  m_moveMaker.unmakeMove();
}
bool BoardManager::isThreeRepetitions(){
  const PosHash posKey = m_board.positionKey;
  int numberOfRepetitions = 0;
  for(int i = m_board.moveCount - m_board.fiftyMoveCounter; i < m_board.moveCount-1;i++){
    if(posKey==m_board.previousMovesArr[i].positionKey)
      numberOfRepetitions++;
  }
  return numberOfRepetitions == 3;
}

GameStatus BoardManager::updateGameStatus(bool noMovesAvalible) {
  bool isKingChecked = m_moveGenerator.isSquareAttackedByEnemy(
      m_board.kingsPositions[m_board.sideToMove]);
  if (noMovesAvalible && isKingChecked) {
    if (m_board.sideToMove == WHITE)
      return GameStatus::BLACK_WIN;
    if (m_board.sideToMove == BLACK)
      return GameStatus::WHITE_WIN;
  }
  if (noMovesAvalible && !isKingChecked)
    return GameStatus::DRAW;
  if (m_board.fiftyMoveCounter == 50)
    return GameStatus::DRAW;
  if (isThreeRepetitions())
    return GameStatus::DRAW;
  return GameStatus::IN_PROGRESS;
}

bool BoardManager::isBoardCorrect() {
  for (int pieceIdx = 0; pieceIdx < 12; pieceIdx++) {
    for (const int pos : m_board.piecesList.getPiecesList(PieceType(pieceIdx))) {
      if (pos == NO_SQUARE)
        continue;
      if (m_board.pieces[pos] != pieceIdx)
        return false;
    }
  }
  for (int pieceIdx = 0; pieceIdx < 120; pieceIdx++) {
    if (m_board.pieces[pieceIdx] > 13)
      return false;
  }
  return true;
}

bool BoardManager::areBoardsSame(const Board &otherBoard) {
  for (int i = 0; i < 120; i++) {
    if (m_board.pieces[i] != otherBoard.pieces[i])
      return false;
  }
  return true;
}