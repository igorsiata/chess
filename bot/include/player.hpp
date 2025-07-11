#include "game_logic/board_manager.hpp"
enum PlayerStatus { WAITING,
                    MOVING };

class Player {
public:
  Player(BoardManager &boardManager) : m_boardManager(boardManager) {}
  void makeMove() { m_boardManager.makeMove(this->chooseMove()); }

protected:
  virtual Move chooseMove();

private:
  BoardManager &m_boardManager;
  PlayerStatus m_playerStatus;
};

class GUIPlayer : public Player {
    public:
        m_board
};