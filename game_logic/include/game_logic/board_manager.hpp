#pragma once

#include "game_logic/board.hpp"
#include "game_logic/fen_parser.hpp"
#include "game_logic/move_generator.hpp"
#include "game_logic/move_maker.hpp"
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
  void makeMove(const Move move);
  void unmakePreviousMove();
  const MoveList generateAllMoves();
  GameStatus getGameStatus() const { return m_gameStatus; }
  bool isWhiteToMove() const { return m_board.sideToMove == WHITE; }
  bool isBoardCorrect();
  bool areBoardsSame(const Board &otherBoard);
  Board &getBoard() { return m_board; }

private:
  Board m_board;
  MoveList m_moveList;
  GameStatus m_gameStatus;
  PositionHasher m_positionHasher;
  MoveGenerator m_moveGenerator;
  MoveMaker m_moveMaker;

  bool isThreeRepetitions();
  void movePiece(const Move &move);
  GameStatus updateGameStatus(bool noMovesAvalible);
};
