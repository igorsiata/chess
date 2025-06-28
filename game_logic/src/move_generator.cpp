#include "game_logic/move_generator.hpp"

MoveGenerator::MoveGenerator(Board &board) : m_board(board) {
}

bool MoveGenerator::isSquareAttackedByEnemy(
    const Position120 square) {

  // Adding 6 to white piece enum gives the same black piece
  int offset = m_board.sideToMove == WHITE ? 6 : 0;
  // Pawn attack
  if (m_board.sideToMove == Color::WHITE) {
    if (m_board.pieces[square - 9] == PieceType::BLACK_PAWN ||
        m_board.pieces[square - 11] == PieceType::BLACK_PAWN)
      return true;
  } else if (m_board.sideToMove == Color::BLACK)
    if (m_board.pieces[square + 9] == PieceType::WHITE_PAWN ||
        m_board.pieces[square + 11] == PieceType::WHITE_PAWN)
      return true;

  // Knight attack
  for (const int move : KnightDir) {
    if (m_board.pieces[square + move] == PieceType::WHITE_KINGHT + offset)
      return true;
  }

  // Slider m_board.pieces
  for (const int move : RookDir) {
    int i = 1;
    while (m_board.pieces[square + move * i] == PieceType::EMPTY)
      i++;
    if (m_board.pieces[square + move * i] == PieceType::WHITE_ROOK + offset ||
        m_board.pieces[square + move * i] == PieceType::WHITE_QUEEN + offset)
      return true;
  }
  for (const int move : BishopDir) {
    int i = 1;
    while (m_board.pieces[square + move * i] == PieceType::EMPTY)
      i++;
    if (m_board.pieces[square + move * i] == PieceType::WHITE_BISHOP + offset ||
        m_board.pieces[square + move * i] == PieceType::WHITE_QUEEN + offset)
      return true;
  }

  // King
  for (const int move : KingQueenDir) {
    if (m_board.pieces[square + move] == PieceType::WHITE_KING + offset)
      return true;
  }

  return false;
}

const MoveList MoveGenerator::generateAllMoves() {
  m_moveList.count = 0;
  GeneratePawnsMoves();
  GenerateAllSliderMoves();
  GenerateKnightMoves();
  GenerateKingMoves();
  addCastles();
  return m_moveList;
}

void MoveGenerator::addMove(Move move) {
  if (isCheckAfterMove(move))
    return;
  m_moveList.moves[m_moveList.count] = move;
  m_moveList.count++;
}

void MoveGenerator::GeneratePawnsMoves() {
  int offset = colorOffset();
  int pawnsIdx = 0 + offset;
  int pawnForward = m_board.sideToMove == Color::WHITE ? -10 : 10;
  std::array<int, 2> pawnAttacks = m_board.sideToMove == Color::WHITE
                                       ? std::array{-11, -9}
                                       : std::array{11, 9};

  for (int pieceIdx = 0; pieceIdx < 10; pieceIdx++) {
    if (m_board.piecesList[pawnsIdx][pieceIdx] == NO_SQUARE)
      continue;
    int piecePos = m_board.piecesList[pawnsIdx][pieceIdx];
    // Move 2 squares
    if (isPawnOnStartSq(piecePos, m_board.sideToMove) &&
        m_board.pieces[piecePos + pawnForward] == EMPTY &&
        m_board.pieces[piecePos + 2 * pawnForward] == EMPTY) {
      Move move = MoveHelper::createMove(piecePos,
                                         piecePos + 2 * pawnForward,
                                         PieceType::EMPTY,
                                         MOVE_FLAG_NO_PROMOTION,
                                         MOVE_FLAG_PAWNSTART);
      addMove(move);
    }

    // Move one square
    if (m_board.pieces[piecePos + pawnForward] == EMPTY) {
      Move move = MoveHelper::createMove(piecePos,
                                         piecePos + pawnForward,
                                         PieceType::EMPTY,
                                         MOVE_FLAG_NO_PROMOTION,
                                         MOVE_FLAG_REGULAR_MOVE);
      promoteIfPossible(move);
    }

    // Captures
    for (int pawnAttack : pawnAttacks) {
      if (m_board.isPieceEnemy(piecePos + pawnAttack)) {

        Move move = MoveHelper::createMove(piecePos,
                                           piecePos + pawnAttack,
                                           m_board.pieces[piecePos + pawnAttack],
                                           MOVE_FLAG_NO_PROMOTION,
                                           MOVE_FLAG_REGULAR_MOVE);
        promoteIfPossible(move);
      }

      // EN Passant
      if (m_board.enPassantSquare != NO_SQUARE &&
          (piecePos + pawnAttack == m_board.enPassantSquare)) {
        Move move = MoveHelper::createMove(piecePos,
                                           piecePos + pawnAttack,
                                           m_board.pieces[piecePos + pawnAttack - pawnForward],
                                           MOVE_FLAG_NO_PROMOTION,
                                           MOVE_FLAG_ENPASSANT);
        addMove(move);
      }
    }
  }
}
void MoveGenerator::promoteIfPossible(Move move) {
  int row = MoveHelper::getEndPos(move) / 10;
  int offset = colorOffset();
  int startPos = MoveHelper::getStartPos(move);
  int endPos = MoveHelper::getEndPos(move);
  int capturedPiece = MoveHelper::getCapturedPiece(move);
  if (row == 2 || row == 9) {

    for (int i = 1; i < 5; i++) {
      uint8_t promotedPiece = i + offset;
      Move promotingMove = MoveHelper::createMove(startPos,
                                                  endPos,
                                                  capturedPiece,
                                                  promotedPiece,
                                                  MOVE_FLAG_REGULAR_MOVE);
      addMove(promotingMove);
    }
    return;
  } else {
    addMove(move);
  }
}

bool MoveGenerator::isPawnOnStartSq(Position120 piecePos, Color color) {
  return (color == WHITE && piecePos > 80 && piecePos < 89) ||
         (color == BLACK && piecePos > 30 && piecePos < 39);
}

void MoveGenerator::GenerateAllSliderMoves(

) {
  const int bishopIdx = m_board.sideToMove == WHITE ? 2 : 8;
  const int rookIdx = m_board.sideToMove == WHITE ? 3 : 9;
  const int queenIdx = m_board.sideToMove == WHITE ? 4 : 10;
  int idx = 0;
  for (const auto piecePos : m_board.piecesList[bishopIdx]) {
    if (piecePos == NO_SQUARE)
      continue;
    GenerateSliderMoves(BishopDir, 4, piecePos);
    idx++;
  }
  idx = 0;
  for (const auto piecePos : m_board.piecesList[rookIdx]) {
    if (piecePos == NO_SQUARE)
      continue;
    GenerateSliderMoves(RookDir, 4, piecePos);
    idx++;
  }
  idx = 0;
  for (const auto piecePos : m_board.piecesList[queenIdx]) {
    if (piecePos == NO_SQUARE)
      continue;
    GenerateSliderMoves(KingQueenDir, 8, piecePos);
    idx++;
  }
}

void MoveGenerator::GenerateSliderMoves(
    const int slidingDir[],
    int slidingDirSize,
    Position120 startSquare) {

  for (int dirIdx = 0; dirIdx < slidingDirSize; dirIdx++) {
    for (int i = 1; i < 9; i++) {
      Position120 endSquare = startSquare + i * slidingDir[dirIdx];
      if (m_board.isPieceEnemy(endSquare)) {
        Move move = MoveHelper::createMove(
            startSquare,
            endSquare,
            m_board.pieces[endSquare],
            MOVE_FLAG_NO_PROMOTION,
            MOVE_FLAG_REGULAR_MOVE);
        addMove(move);
        break;
      }
      if (m_board.isPieceAlly(endSquare) || m_board.pieces[endSquare] == OFF_BOARD) {
        break;
      }
      Move move = MoveHelper::createMove(
          startSquare,
          endSquare,
          m_board.pieces[endSquare],
          MOVE_FLAG_NO_PROMOTION,
          MOVE_FLAG_REGULAR_MOVE);
      addMove(move);
    }
  }
}

void MoveGenerator::GenerateKnightMoves() {
  int knightIdx = m_board.sideToMove == WHITE ? 1 : 7;
  for (int pieceIdx = 0; pieceIdx < 10; pieceIdx++) {
    if (m_board.piecesList[knightIdx][pieceIdx] == NO_SQUARE)
      continue;
    int piecePos = m_board.piecesList[knightIdx][pieceIdx];
    for (const int move : KnightDir) {
      if (m_board.pieces[piecePos + move] == EMPTY ||
          m_board.isPieceEnemy(piecePos + move)) {
        Move moveToAdd = MoveHelper::createMove(
            piecePos,
            piecePos + move,
            m_board.pieces[piecePos + move],
            MOVE_FLAG_NO_PROMOTION,
            MOVE_FLAG_REGULAR_MOVE);
        addMove(moveToAdd);
      }
    }
  }
}

void MoveGenerator::GenerateKingMoves() {
  Position120 kingPos = m_board.kingsPositions[m_board.sideToMove];
  for (const int move : KingQueenDir) {
    if (m_board.pieces[kingPos + move] == EMPTY ||
        m_board.isPieceEnemy(kingPos + move)) {
      Move moveToAdd = MoveHelper::createMove(
          kingPos,
          kingPos + move,
          m_board.pieces[kingPos + move],
          MOVE_FLAG_NO_PROMOTION,
          MOVE_FLAG_REGULAR_MOVE);
      addMove(moveToAdd);
    }
  }
}

bool MoveGenerator::isCheckAfterMove(Move move) {
  const auto piecesCopy = m_board.pieces;
  PieceType pieceToMove = m_board.pieces[MoveHelper::getStartPos(move)];
  m_board.pieces[MoveHelper::getStartPos(move)] = PieceType::EMPTY;
  m_board.pieces[MoveHelper::getEndPos(move)] = pieceToMove;
  if (MoveHelper::isMoveEnPassant(move)) {
    int capturedPieceSquare = MoveHelper::getEndPos(move);
    capturedPieceSquare += m_board.sideToMove == WHITE ? 10 : -10;
    m_board.pieces[capturedPieceSquare] = PieceType::EMPTY;
  }
  if (pieceToMove == WHITE_KING || pieceToMove == BLACK_KING) {
    m_board.kingsPositions[m_board.sideToMove] = MoveHelper::getEndPos(move);
  }
  bool checkAfterMove = isSquareAttackedByEnemy(m_board.kingsPositions[m_board.sideToMove]);

  if (MoveHelper::isMoveEnPassant(move)) {
    int capturedPieceSquare = MoveHelper::getEndPos(move);
    capturedPieceSquare += m_board.sideToMove == WHITE ? 10 : -10;
    m_board.pieces[capturedPieceSquare] = PieceType(MoveHelper::getCapturedPiece(move));
    m_board.pieces[MoveHelper::getEndPos(move)] = PieceType::EMPTY;
  } else {
    m_board.pieces[MoveHelper::getEndPos(move)] = PieceType(MoveHelper::getCapturedPiece(move));
  }
  if (pieceToMove == WHITE_KING || pieceToMove == BLACK_KING) {
    m_board.kingsPositions[m_board.sideToMove] = MoveHelper::getStartPos(move);
  }
  m_board.pieces[MoveHelper::getStartPos(move)] = pieceToMove;
  for (int i = 0; i < 120; i++) {
    if (m_board.pieces[i] != piecesCopy[i])
      return false;
  }
  return checkAfterMove;
}

void MoveGenerator::addCastles() {
  int castleQueenside;
  int castleKingside;
  int kingPos;
  if (m_board.sideToMove == WHITE) {
    castleQueenside = CastleRights::WHITE_QUEENSIDE;
    castleKingside = CastleRights::WHITE_KINGSIDE;
    kingPos = 95;
  } else {
    castleQueenside = CastleRights::BLACK_QUEENSIDE;
    castleKingside = CastleRights::BLACK_KINGSIDE;
    kingPos = 25;
  }
  bool canCastleQueenside = (m_board.castleRights & castleQueenside) != 0;
  bool canCastleKingside = (m_board.castleRights & castleKingside) != 0;
  if (isSquareAttackedByEnemy(kingPos))
    return;
  if (canCastleQueenside) {
    if (!isSquareAttackedByEnemy(kingPos - 1) &&
        !isSquareAttackedByEnemy(kingPos - 2) &&
        m_board.pieces[kingPos - 1] == PieceType::EMPTY &&
        m_board.pieces[kingPos - 2] == PieceType::EMPTY &&
        m_board.pieces[kingPos - 3] == PieceType::EMPTY) {

      Move move = MoveHelper::createMove(
          kingPos,
          kingPos - 2,
          PieceType::EMPTY,
          MOVE_FLAG_NO_PROMOTION,
          MOVE_FLAG_CASTLE);
      addMove(move);
    }
  }
  if (canCastleKingside) {
    if (!isSquareAttackedByEnemy(kingPos + 1) &&
        !isSquareAttackedByEnemy(kingPos + 2) &&
        m_board.pieces[kingPos + 1] == PieceType::EMPTY &&
        m_board.pieces[kingPos + 2] == PieceType::EMPTY) {
      Move move = MoveHelper::createMove(
          kingPos,
          kingPos + 2,
          PieceType::EMPTY,
          MOVE_FLAG_NO_PROMOTION,
          MOVE_FLAG_CASTLE);
      addMove(move);
    }
  }
}
