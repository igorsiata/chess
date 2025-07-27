#pragma once

#include <memory>
#include "game_logic/board_manager.hpp"

enum PlayerStatus { WAITING,
                    MOVING };

class Player {
public:
  void makeMove(BoardManager& boardManager) {
    m_playerStatus = PlayerStatus::MOVING;
    boardManager.makeMove(this->chooseMove(boardManager.getBoard()));
    m_playerStatus = PlayerStatus::WAITING;
  }
  PlayerStatus getPlayerStatus(){return m_playerStatus;}
  virtual ~Player() = default;
protected:
  virtual Move chooseMove(Board &board) = 0;

private:
  PlayerStatus m_playerStatus;
};

// class GUIPlayer : public Player {
//     public:
//         m_boardManager 
// };