#pragma once

#include "game_logic/board.hpp"
#include "game_logic/fen_parser.hpp"
#include "game_logic/move_generator.hpp"
#include "game_logic/position_hasher.hpp"

enum GameStatus {
  IN_PROGRESS,
  DRAW,
  WHITE_WIN,
  BLACK_WIN
};

class BoardManager {
public:
  BoardManager();
  void resetBoard();
  void loadBoardFromFEN(const std::string &startPositionFEN);
  void makeMove(const Move &move);
  void unmakePreviousMove();
  MoveList generateAllMoves();
  GameStatus getGameStatus() const { return m_gameStatus; }
  bool isWhiteToMove() const { return m_board.sideToMove == WHITE; }
  const PieceType *getPieces() const { return m_board.pieces; };
  int getFullMoveCount() const { return m_board.moveCount; }
  bool isBoardCorrect();
  bool areBoardsSame(const Board &otherBoard);
  Board m_board;

private:
  GameStatus m_gameStatus;

  void movePiece(const Move &move);
  void removeCastleRights(const Move &move);
  void capturePiece(const Move &move);
  void updateCounters(const Move &move);
  void makeCastle(const Move &move);
  void promotePawn(const Move &move);
  GameStatus updateGameStatus(bool noMovesAvalible);
  PositionHasher m_positionHasher;
  MoveGenerator m_moveGenerator;
};
