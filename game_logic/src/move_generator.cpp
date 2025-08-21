#include "game_logic/move_generator.hpp"

MoveGenerator::MoveGenerator(Board &board, MoveList &moveList) : m_board(board), m_moveList(moveList) {
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

void MoveGenerator::getPinnedPieces() {
  pinnedPieces.clear();
  const Position120 kingSquare = m_board.kingsPositions[m_board.sideToMove];
  const int offset = m_board.sideToMove == WHITE ? 6 : 0;
  const int allyPieces = m_board.sideToMove == WHITE ? 6 : 12;
  // Slider m_board.pieces
  for (const int move : RookDir) {
    int i = 1;
    Position120 pinnedPiece = EMPTY;
    while (m_board.pieces[kingSquare + move * i] == PieceType::EMPTY)
      i++;
    if (m_board.pieces[kingSquare + move * i] < allyPieces)
      pinnedPiece = kingSquare + move * i;
    i++;
    while (m_board.pieces[kingSquare + move * i] == PieceType::EMPTY)
      i++;
    if (m_board.pieces[kingSquare + move * i] == PieceType::WHITE_ROOK + offset ||
        m_board.pieces[kingSquare + move * i] == PieceType::WHITE_QUEEN + offset)
      pinnedPieces.insert(pinnedPiece);
  }
  for (const int move : BishopDir) {
    int i = 1;
    Position120 pinnedPiece = EMPTY;
    while (m_board.pieces[kingSquare + move * i] == PieceType::EMPTY)
      i++;
    if (m_board.pieces[kingSquare + move * i] < allyPieces)
      pinnedPiece = kingSquare + move * i;
    i++;
    while (m_board.pieces[kingSquare + move * i] == PieceType::EMPTY)
      i++;
    if (m_board.pieces[kingSquare + move * i] == PieceType::WHITE_BISHOP + offset ||
        m_board.pieces[kingSquare + move * i] == PieceType::WHITE_QUEEN + offset)
      pinnedPieces.insert(pinnedPiece);
  }
}

void MoveGenerator::generateAllMoves() {
  m_moveList.count = 0;
  isKingInCheck = isSquareAttackedByEnemy(m_board.kingsPositions[m_board.sideToMove]);
  getPinnedPieces();
  GeneratePawnsMoves();
  GenerateAllSliderMoves();
  GenerateKnightMoves();
  GenerateKingMoves();
  addCastles();
}

bool MoveGenerator::isPieceEnemy(Position120 piecePos) {
  PieceType otherPiece = m_board.pieces[piecePos];
  if (m_board.sideToMove == Color::WHITE)
    return otherPiece > 5 && otherPiece < 12;
  else
    return otherPiece < 6;
}

void MoveGenerator::addMove(Move move) {
  // if (pinnedPieces.find(move.startPos()) != pinnedPieces.end() ||
  //     move.isEnPassant() ||
  //     isKingInCheck ||
  //     move.startPos() == m_board.kingsPositions[m_board.sideToMove])
  if (isCheckAfterMove(move))
    return;

  m_moveList.moves[m_moveList.count] = move;
  m_moveList.count++;
}

void MoveGenerator::GeneratePawnsMoves() {
  PieceType pawnsIdx = PieceType(PieceType::WHITE_PAWN + colorOffset());
  int pawnforward = m_board.sideToMove == Color::WHITE ? -10 : 10;
  std::array<int, 2> pawnattacks = m_board.sideToMove == Color::WHITE
                                       ? std::array{-11, -9}
                                       : std::array{11, 9};

  for (int piecePos : m_board.piecesList.getPiecesList(pawnsIdx)) {
    if (piecePos == NO_SQUARE)
      continue;
    // move 2 squares
    if (isPawnOnStartSq(piecePos, m_board.sideToMove) &&
        m_board.pieces[piecePos + pawnforward] == EMPTY &&
        m_board.pieces[piecePos + 2 * pawnforward] == EMPTY) {
      Move move(piecePos,
                piecePos + 2 * pawnforward,
                PieceType::EMPTY,
                MOVE_FLAG_NO_PROMOTION,
                MOVE_FLAG_PAWNSTART);
      addMove(move);
    }

    // move one square
    if (m_board.pieces[piecePos + pawnforward] == EMPTY) {
      Move move(piecePos,
                piecePos + pawnforward,
                PieceType::EMPTY,
                MOVE_FLAG_NO_PROMOTION,
                MOVE_FLAG_REGULAR_MOVE);
      promoteIfPossible(move);
    }

    // captures
    for (int pawnAttack : pawnattacks) {
      if (isPieceEnemy(piecePos + pawnAttack)) {

        Move move(piecePos,
                  piecePos + pawnAttack,
                  m_board.pieces[piecePos + pawnAttack],
                  MOVE_FLAG_NO_PROMOTION,
                  MOVE_FLAG_REGULAR_MOVE);
        promoteIfPossible(move);
      }

      // en passant
      if (m_board.enPassantSquare != NO_SQUARE &&
          (piecePos + pawnAttack == m_board.enPassantSquare)) {
        Move move(piecePos,
                  piecePos + pawnAttack,
                  m_board.pieces[piecePos + pawnAttack - pawnforward],
                  MOVE_FLAG_NO_PROMOTION,
                  MOVE_FLAG_ENPASSANT);
        addMove(move);
      }
    }
  }
}
void MoveGenerator::promoteIfPossible(Move move) {
  int row = move.endPos() / 10;
  int offset = colorOffset();
  int startPos = move.startPos();
  int endPos = move.endPos();
  int capturedPiece = move.capturedPiece();
  if (row == 2 || row == 9) {

    for (int i = 1; i < 5; i++) {
      uint8_t promotedPiece = i + offset;
      Move promotingMove(startPos,
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

void MoveGenerator::GenerateAllSliderMoves() {
  const PieceType bishopIdx = m_board.sideToMove == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
  const PieceType rookIdx = m_board.sideToMove == WHITE ? WHITE_ROOK : BLACK_ROOK;
  const PieceType queenIdx = m_board.sideToMove == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
  int idx = 0;
  for (const auto piecePos : m_board.piecesList.getPiecesList(bishopIdx)) {
    if (piecePos == NO_SQUARE)
      continue;
    GenerateSliderMoves(BishopDir, 4, piecePos);
    idx++;
  }
  idx = 0;
  for (const auto piecePos : m_board.piecesList.getPiecesList(rookIdx)) {
    if (piecePos == NO_SQUARE)
      continue;
    GenerateSliderMoves(RookDir, 4, piecePos);
    idx++;
  }
  idx = 0;
  for (const auto piecePos : m_board.piecesList.getPiecesList(queenIdx)) {
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
      if (isPieceEnemy(endSquare)) {
        Move move(
            startSquare,
            endSquare,
            m_board.pieces[endSquare],
            MOVE_FLAG_NO_PROMOTION,
            MOVE_FLAG_REGULAR_MOVE);
        addMove(move);
      }
      if (m_board.pieces[endSquare] != EMPTY) {
        break;
      }
      Move move(
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
  PieceType knightIdx = m_board.sideToMove == WHITE ? WHITE_KINGHT : BLACK_KINGHT;
  for (int piecePos : m_board.piecesList.getPiecesList(knightIdx)) {
    if (piecePos == NO_SQUARE)
      continue;
    for (const int move : KnightDir) {
      if (m_board.pieces[piecePos + move] == EMPTY ||
          isPieceEnemy(piecePos + move)) {
        Move moveToAdd(
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
    const bool isValidSquare = m_board.pieces[kingPos + move] == EMPTY ||
                               isPieceEnemy(kingPos + move);
    if (isValidSquare && !isSquareAttackedByEnemy(kingPos + move)) {
      Move moveToAdd(
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
  const Position120 startPos = move.startPos();
  const Position120 endPos = move.endPos();
  const PieceType pieceToMove = m_board.pieces[startPos];
  const int enPassAdd = m_board.sideToMove == WHITE ? 10 : -10;
  m_board.pieces[startPos] = PieceType::EMPTY;
  m_board.pieces[endPos] = pieceToMove;
  if (move.isEnPassant()) {
    int capturedPieceSquare = endPos + enPassAdd;
    m_board.pieces[capturedPieceSquare] = PieceType::EMPTY;
  }
  if (pieceToMove == WHITE_KING || pieceToMove == BLACK_KING) {
    m_board.kingsPositions[m_board.sideToMove] = endPos;
  }
  bool checkAfterMove = isSquareAttackedByEnemy(m_board.kingsPositions[m_board.sideToMove]);

  if (move.isEnPassant()) {
    int capturedPieceSquare = endPos + enPassAdd;
    m_board.pieces[capturedPieceSquare] = PieceType(move.capturedPiece());
    m_board.pieces[endPos] = PieceType::EMPTY;
  } else {
    m_board.pieces[endPos] = PieceType(move.capturedPiece());
  }
  if (pieceToMove == WHITE_KING || pieceToMove == BLACK_KING) {
    m_board.kingsPositions[m_board.sideToMove] = startPos;
  }
  m_board.pieces[startPos] = pieceToMove;

  return checkAfterMove;
}

void MoveGenerator::addCastles() {
  int castleQueenside;
  int castleKingside;
  int kingPos;
  if (m_board.sideToMove == WHITE) {
    castleQueenside = CASTLE_WHITE_QUEENSIDE;
    castleKingside = CASTLE_WHITE_KINGSIDE;
    kingPos = 95;
  } else {
    castleQueenside = CASTLE_BLACK_QUEENSIDE;
    castleKingside = CASTLE_BLACK_KINGSIDE;
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

      Move move(
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
      Move move(
          kingPos,
          kingPos + 2,
          PieceType::EMPTY,
          MOVE_FLAG_NO_PROMOTION,
          MOVE_FLAG_CASTLE);
      addMove(move);
    }
  }
}
