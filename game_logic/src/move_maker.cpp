#include "game_logic/move_maker.hpp"
void MoveMaker::makeMove(const Move move) {
  const Position120 startSqr = move.startPos();
  const Position120 endSqr = move.endPos();
  const PieceType capturedPiece = PieceType(move.capturedPiece());
  const bool isWhiteMove = m_board.sideToMove == Color::WHITE;
  const bool isCapture = capturedPiece != PieceType::EMPTY;
  const PieceType movedPiece = m_board.pieces[startSqr];

  PreviousMove previousMove = {
      move,
      m_board.enPassantSquare,
      m_board.fiftyMoveCounter,
      m_board.castleRights,
      m_board.positionKey};
  m_board.previousMovesArr[m_board.moveCount] = previousMove;

  if (isCapture && !move.isEnPassant()) {
    clearPiece(endSqr);
  }
  if (move.isCastle()) {
    const bool isKingSide = BoardHelper::getFile(endSqr) == 6;
    const Position120 rookStartSqr = startSqr + (isKingSide ? 3 : -4);
    const Position120 rookEndSqr = startSqr + (isKingSide ? 1 : -1);
    movePiece(rookStartSqr, rookEndSqr);
  }
  if (move.isEnPassant()) {
    const Position120 capturedPieceSqr = endSqr + (isWhiteMove ? 10 : -10);
    clearPiece(capturedPieceSqr);
  }

  if (m_board.enPassantSquare != NO_SQUARE) {
    m_positionHasher.hashEnPassSqr(m_board.positionKey, m_board.enPassantSquare); // add new
  }
  if (startSqr == m_board.kingsPositions[m_board.sideToMove]) {
    m_board.kingsPositions[m_board.sideToMove] = endSqr;
  }
  if (move.isPawnStart()) {
    m_board.enPassantSquare = endSqr + (isWhiteMove ? 10 : -10);
    m_positionHasher.hashEnPassSqr(m_board.positionKey, m_board.enPassantSquare);
  } else {
    m_board.enPassantSquare = NO_SQUARE;
  }
  if (move.isPromotion()) {
    const PieceType promotedPiece = PieceType(move.promotedPiece());
    clearPiece(startSqr);
    addPiece(promotedPiece, endSqr);
  } else {
    movePiece(startSqr, endSqr);
  }

  m_board.sideToMove = isWhiteMove ? BLACK : WHITE;
  m_board.moveCount++;
  m_board.fiftyMoveCounter++;
  const bool isPawnMove = (movedPiece == WHITE_PAWN || movedPiece == BLACK_PAWN);
  if (isPawnMove || isCapture)
    m_board.fiftyMoveCounter = 0;
  m_positionHasher.hashCastle(m_board.positionKey, m_board.castleRights);
  m_board.castleRights &= (castlePermisions[BoardHelper::pos120to64(startSqr)] &
                           castlePermisions[BoardHelper::pos120to64(endSqr)]);
  m_positionHasher.hashCastle(m_board.positionKey, m_board.castleRights);
  m_positionHasher.hashSide(m_board.positionKey);
  // m_board.positionKeysMap[m_board.positionKey]++;
}

void MoveMaker::unmakeMove() {
  const PreviousMove previousMove = m_board.previousMovesArr[m_board.moveCount - 1];
  const Move move = previousMove.move;
  const Position120 startSqr = move.startPos();
  const Position120 endSqr = move.endPos();
  const PieceType capturedPiece = PieceType(move.capturedPiece());
  const bool isCapture = capturedPiece != PieceType::EMPTY;
  const bool wasWhiteMoving = m_board.sideToMove == Color::BLACK;

  m_board.castleRights = previousMove.castleRights;
  m_board.fiftyMoveCounter = previousMove.fiftyMoveCounter;
  // m_board.positionKeysMap[m_board.positionKey]--;
  m_board.sideToMove = wasWhiteMoving ? Color::WHITE : Color::BLACK;
  m_board.enPassantSquare = previousMove.enPassantSquare;
  m_board.moveCount--;

  // Modifying poshash in clear, add, move functions. Will assign accual at end of fnction.

  movePiece(endSqr, startSqr);

  if (isCapture && !move.isEnPassant()) {
    addPiece(capturedPiece, endSqr);
  }
  if (move.isCastle()) {
    const bool isKingSide = BoardHelper::getFile(endSqr) == 6;
    const Position120 rookStartSqr = startSqr + (isKingSide ? 3 : -4);
    const Position120 rookEndSqr = startSqr + (isKingSide ? 1 : -1);
    movePiece(rookEndSqr, rookStartSqr);
  }
  if (move.isEnPassant()) {
    const Position120 capturedPieceSqr = endSqr + (wasWhiteMoving ? 10 : -10);
    addPiece(capturedPiece, capturedPieceSqr);
  }
  if (move.isPromotion()) {
    clearPiece(startSqr);
    const PieceType pawnType = wasWhiteMoving ? WHITE_PAWN : BLACK_PAWN;
    addPiece(pawnType, startSqr);
  }
  if (m_board.kingsPositions[!wasWhiteMoving] == endSqr) {
    m_board.kingsPositions[!wasWhiteMoving] = startSqr;
  }
  m_board.positionKey = previousMove.positionKey;
}

void MoveMaker::clearPiece(const Position120 sqr) {
  const PieceType pieceType = m_board.pieces[sqr];
  m_positionHasher.hashPiece(m_board.positionKey, pieceType, sqr);
  m_board.piecesList.removePiece(pieceType, sqr);
  m_board.pieces[sqr] = PieceType::EMPTY;
}

void MoveMaker::addPiece(const PieceType pieceType, const Position120 sqr) {
  m_board.pieces[sqr] = pieceType;
  m_board.piecesList.addPiece(pieceType, sqr);
  m_positionHasher.hashPiece(m_board.positionKey, pieceType, sqr);
}

void MoveMaker::movePiece(const Position120 startSqr, const Position120 endSqr) {
  const PieceType pieceType = m_board.pieces[startSqr];
  m_positionHasher.hashPiece(m_board.positionKey, pieceType, startSqr);
  m_positionHasher.hashPiece(m_board.positionKey, pieceType, endSqr);
  m_board.pieces[startSqr] = PieceType::EMPTY;
  m_board.pieces[endSqr] = pieceType;
  m_board.piecesList.changePiecePos(pieceType, startSqr, endSqr);
}