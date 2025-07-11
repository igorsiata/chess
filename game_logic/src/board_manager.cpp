#include "game_logic/board_manager.hpp"

BoardManager::BoardManager() : m_moveGenerator(m_board) {

  resetBoard();
}

void BoardManager::loadBoardFromFEN(const std::string &positionFEN) {

  resetBoard();
  FENParser::loadPosition(positionFEN, &m_board);
  m_board.positionKey = m_positionHasher.hashPosition(m_board);
  m_board.positionKeysMap[m_board.positionKey] += 1;
}
MoveList BoardManager::generateAllMoves() {
  const MoveList moveList = m_moveGenerator.generateAllMoves();
  m_gameStatus = updateGameStatus(moveList.count == 0);
  return moveList;
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

  m_board.previousMovesVec.clear();
  m_board.previousMovesVec.reserve(256);
  m_board.moveCount = 0;
  m_board.fiftyMoveCounter = 0;
  m_board.castleRights = CastleRights(0);
  m_board.positionKey = uint64_t(0);
  m_board.positionKeysMap.clear();
}

void BoardManager::makeMove(const Move &move) {

  PreviousMove previousMove = {
      move, m_board.enPassantSquare, m_board.fiftyMoveCounter, m_board.castleRights, m_board.positionKey};
  m_board.previousMovesVec.push_back((previousMove));
  removeCastleRights(move);
  capturePiece(move);
  movePiece(move);
  promotePawn(move);
  makeCastle(move);
  updateCounters(move);
}

void BoardManager::unmakePreviousMove() {

  PreviousMove previousMove = m_board.previousMovesVec.back();
  m_board.previousMovesVec.pop_back();

  m_board.castleRights = previousMove.castleRights;
  m_board.fiftyMoveCounter = previousMove.fiftyMoveCounter;
  m_board.positionKeysMap[m_board.positionKey] -= 1;
  m_board.positionKey = previousMove.positionKey;
  m_board.sideToMove = m_board.sideToMove == WHITE ? BLACK : WHITE;
  m_board.enPassantSquare = previousMove.enPassantSquare;
  m_board.moveCount -= 1;

  Move move = previousMove.move;
  int endPos = MoveHelper::getEndPos(move);
  int startPos = MoveHelper::getStartPos(move);
  PieceType capturedPiece = PieceType(MoveHelper::getCapturedPiece(move));

  PieceType movedPiece = m_board.pieces[endPos];
  if (movedPiece == EMPTY) {
    std::cout << "ERROR: Moved piece is empty\n";
  }

  // promotion
  if (MoveHelper::isMovePromotion(move)) {
    PieceType promotedPiece = m_board.pieces[endPos];
    m_board.piecesList.removePiece(promotedPiece, endPos);
    movedPiece = m_board.sideToMove == WHITE ? WHITE_PAWN : BLACK_PAWN;
    m_board.piecesList.addPiece(movedPiece, endPos);
  }

  // unmake move
  m_board.pieces[startPos] = movedPiece;
  m_board.piecesList.changePiecePos(movedPiece, endPos, startPos);

  // capture
  int capturedPieceSquare = MoveHelper::getEndPos(move);
  if (MoveHelper::isMoveEnPassant(move)) {
    capturedPieceSquare += m_board.sideToMove == WHITE ? 10 : -10;
    m_board.pieces[endPos] = PieceType::EMPTY;
  }
  m_board.pieces[capturedPieceSquare] = PieceType(capturedPiece);
  if (capturedPiece != EMPTY) {
    m_board.piecesList.addPiece(capturedPiece, capturedPieceSquare);
  }

  // castle
  if (MoveHelper::isMoveCastle(move)) {
    Position120 kingPos = m_board.kingsPositions[m_board.sideToMove];
    if (kingPos % 10 == 3) {
      Move rookMove = MoveHelper::createMove(kingPos + 1, kingPos - 2, PieceType::EMPTY, 0, 0);
      movePiece(rookMove);
    } else if (kingPos % 10 == 7) {
      Move rookMove = MoveHelper::createMove(kingPos - 1, kingPos + 1, PieceType::EMPTY, 0, 0);
      movePiece(rookMove);
    }
  }
  if (MoveHelper::getEndPos(move) == m_board.kingsPositions[m_board.sideToMove])
    m_board.kingsPositions[m_board.sideToMove] = startPos;
}

void BoardManager::updateCounters(const Move &move) {
  int startPos = MoveHelper::getStartPos(move);
  m_board.sideToMove = m_board.sideToMove == WHITE ? BLACK : WHITE;
  m_board.moveCount += 1;
  if (MoveHelper::getCapturedPiece(move) != EMPTY ||
      m_board.pieces[startPos] == PieceType::WHITE_PAWN ||
      m_board.pieces[startPos] == PieceType::BLACK_PAWN)
    m_board.fiftyMoveCounter = 0;
  else
    m_board.fiftyMoveCounter += 1;
  m_board.positionKey = m_positionHasher.hashPosition(m_board);
  m_board.positionKeysMap[m_board.positionKey] += 1;
}

void BoardManager::makeCastle(const Move &move) {
  if (MoveHelper::isMoveCastle(move)) {
    bool castleQueenside = MoveHelper::getStartPos(move) > MoveHelper::getEndPos(move);
    if (castleQueenside) {
      Move rookMove = MoveHelper::createMove(
          MoveHelper::getEndPos(move) - 2,
          MoveHelper::getEndPos(move) + 1,
          PieceType::EMPTY,
          0, 0);
      movePiece(rookMove);
    } else {
      Move rookMove = MoveHelper::createMove(
          MoveHelper::getEndPos(move) + 1,
          MoveHelper::getEndPos(move) - 1,
          PieceType::EMPTY,
          0, 0);
      movePiece(rookMove);
    }
  }
}

void BoardManager::movePiece(const Move &move) {
  if (MoveHelper::isMovePromotion(move))
    return;
  int startPos = MoveHelper::getStartPos(move);
  int endPos = MoveHelper::getEndPos(move);
  PieceType movedPiece = m_board.pieces[startPos];
  m_board.piecesList.changePiecePos(movedPiece, startPos, endPos);
  // King move
  if (startPos == m_board.kingsPositions[m_board.sideToMove])
    m_board.kingsPositions[m_board.sideToMove] = endPos;

  // Add enPassant square
  if (MoveHelper::isMovePawnStart(move))
    m_board.enPassantSquare = (endPos + startPos) / 2;
  else
    m_board.enPassantSquare = NO_SQUARE;
  m_board.pieces[startPos] = PieceType::EMPTY;
  m_board.pieces[endPos] = movedPiece;
}

void BoardManager::removeCastleRights(const Move &move) {
  // Remove castle rights
  int startPos = MoveHelper::getStartPos(move);
  if (startPos == m_board.kingsPositions[m_board.sideToMove]) {
    m_board.castleRights &= m_board.sideToMove == WHITE ? 0b1100 : 0b0011;
  }
  // WhiteKingSide, WhiteQueenSide, BlackKingSide, BlackQueenSide,
  const uint8_t rookSquares[4] = {98, 91, 28, 21};
  const int removeCastleMasks[4] = {0b1110, 0b1101, 0b1011, 0b0111};
  for (uint8_t i = 0; i < 4; i++) {
    if (startPos == rookSquares[i] || MoveHelper::getEndPos(move) == rookSquares[i]) {
      m_board.castleRights &= removeCastleMasks[i];
    }
  }
}

void BoardManager::capturePiece(const Move &move) {
  PieceType capturedPiece = PieceType(MoveHelper::getCapturedPiece(move));
  int capturedPieceSquare = MoveHelper::getEndPos(move);
  if (MoveHelper::isMoveEnPassant(move)) {
    capturedPieceSquare += m_board.sideToMove == WHITE ? 10 : -10;
    m_board.pieces[capturedPieceSquare] = PieceType::EMPTY;
  }

  if (capturedPiece != EMPTY) {
    m_board.piecesList.removePiece(capturedPiece, capturedPieceSquare);
  }
}

void BoardManager::promotePawn(const Move &move) {
  if (!MoveHelper::isMovePromotion(move))
    return;
  int startPos = MoveHelper::getStartPos(move);
  int endPos = MoveHelper::getEndPos(move);
  PieceType movedPiece = PieceType(m_board.pieces[startPos]);
  PieceType promotedPiece = PieceType(MoveHelper::getPromotedPiece(move));
  m_board.piecesList.removePiece(movedPiece, startPos);
  m_board.piecesList.addPiece(promotedPiece, endPos);
  m_board.pieces[endPos] = promotedPiece;
  m_board.pieces[startPos] = PieceType::EMPTY;
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
  for (const auto &pair : m_board.positionKeysMap) {
    if (pair.second >= 3)
      return GameStatus::DRAW;
  }
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