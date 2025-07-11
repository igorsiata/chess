#include "game_logic/fen_parser.hpp"

void FENParser::loadPosition(const std::string &positionFEN, Board *board) {
  FENParser::loadPieces(positionFEN, board);
  FENParser::loadPlayerToMove(positionFEN, board);
  FENParser::loadAvailableCastles(positionFEN, board);
  FENParser::loadEnPassantSquare(positionFEN, board);
  FENParser::loadCounters(positionFEN, board);
}

void FENParser::loadPieces(const std::string &positionFEN, Board *board) {
  uint8_t currentPosition64 = 0;
  Position120 currentPosition120 = BoardHelper::pos64to120(currentPosition64);
  std::map<char, PieceType> pieceMap = {
      {'P', PieceType::WHITE_PAWN},
      {'N', PieceType::WHITE_KINGHT},
      {'B', PieceType::WHITE_BISHOP},
      {'R', PieceType::WHITE_ROOK},
      {'Q', PieceType::WHITE_QUEEN},
      {'K', PieceType::WHITE_KING},
      {'p', PieceType::BLACK_PAWN},
      {'n', PieceType::BLACK_KINGHT},
      {'b', PieceType::BLACK_BISHOP},
      {'r', PieceType::BLACK_ROOK},
      {'q', PieceType::BLACK_QUEEN},
      {'k', PieceType::BLACK_KING},
  };
  for (const char it : positionFEN) {
    if (it == ' ') {
      break;
    }
    if (it == '/') {
      currentPosition64--;
    } else if (std::isdigit(it)) {
      currentPosition64 += uint8_t(it - 48) - 1;
    } else {
      PieceType piece = pieceMap[it];
      board->pieces[currentPosition120] = piece;
      
      board->piecesList.addPiece(piece, currentPosition120);
      if (it == 'K') {
        board->kingsPositions[0] = currentPosition120;
      }
      if (it == 'k') {
        board->kingsPositions[1] = currentPosition120;
      }
    }
    currentPosition64++;
    currentPosition120 = BoardHelper::pos64to120(currentPosition64);
  }
}

void FENParser::loadPlayerToMove(const std::string &positionFEN, Board *board) {
  int firstSpaceIdx = positionFEN.find(' ');
  char playerToMove = positionFEN[firstSpaceIdx + 1];
  if (playerToMove == 'w') {
    board->sideToMove = Color::WHITE;
  } else if (playerToMove == 'b') {
    board->sideToMove = Color::BLACK;
  }
}

void FENParser::loadAvailableCastles(const std::string &positionFEN, Board *board) {
  size_t firstSpaceIdx = positionFEN.find(' ');
  size_t secondSpaceIdx = positionFEN.find(' ', firstSpaceIdx + 1);
  uint8_t castleRights = 0;
  if (firstSpaceIdx == std::string::npos || secondSpaceIdx == std::string::npos) {
    return;
  }
  for (auto it : positionFEN.substr(secondSpaceIdx + 1)) {
    if (it == ' ')
      break;
    if (it == 'K')
      castleRights += 1;
    if (it == 'Q')
      castleRights += 2;
    if (it == 'k')
      castleRights += 4;
    if (it == 'q')
      castleRights += 8;
  }
  board->castleRights = castleRights;
}

void FENParser::loadEnPassantSquare(const std::string &positionFEN, Board *board) {
  size_t firstSpaceIdx = positionFEN.find(' ');
  size_t secondSpaceIdx = positionFEN.find(' ', firstSpaceIdx + 1);
  size_t thirdSpaceIdx = positionFEN.find(' ', secondSpaceIdx + 1);
  if (firstSpaceIdx == std::string::npos || secondSpaceIdx == std::string::npos || thirdSpaceIdx == std::string::npos) {
    return;
  }
  auto subStr = positionFEN.substr(thirdSpaceIdx + 1);
  if (subStr[0] != '-') {
    uint8_t file = subStr[0] - 97;
    uint8_t rank = subStr[1] - 48;
    Position64 pos64 = (8 - rank) * 8 + file;
    board->enPassantSquare = BoardHelper::pos64to120(pos64);
  }else{
    board->enPassantSquare = NO_SQUARE;
  }
}

void FENParser::loadCounters(const std::string &positionFEN, Board *board) {
  size_t spacesIdx[5];
  spacesIdx[0] = positionFEN.find(' ');
  for (int i = 1; i < 5; i++) {
    spacesIdx[i] = positionFEN.find(' ', spacesIdx[i - 1] + 1);
    if (spacesIdx[i] == std::string::npos)
      return;
  }
  auto subStrHalfMove = positionFEN.substr(spacesIdx[3] + 1, spacesIdx[4] - spacesIdx[3]);
  auto subStrFullMove = positionFEN.substr(spacesIdx[4] + 1);
  board->fiftyMoveCounter = std::stoi(subStrHalfMove);
  board->moveCount = std::stoi(subStrFullMove);
}