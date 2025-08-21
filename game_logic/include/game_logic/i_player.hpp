#pragma once

#include <memory>
#include <thread>
#include "game_logic/board_manager.hpp"

enum PlayerStatus { WAITING,
                    MOVING };

class IPlayer {
public:
  bool makeMove(BoardManager& boardManager) {
    m_playerStatus = PlayerStatus::MOVING;
    Move move = this->chooseMove(boardManager);
    if (!move){
      boardManager.makeMove(move);
      m_playerStatus = PlayerStatus::WAITING;
      return true;
    }
    return false;
  }
  PlayerStatus getPlayerStatus(){return m_playerStatus;}
  virtual ~IPlayer() = default;
protected:
  virtual Move chooseMove(BoardManager &boardManager) = 0;

private:
  PlayerStatus m_playerStatus = WAITING;
};

// class GUIPlayer : public Player {
//     public:
//         m_boardManager 
// };