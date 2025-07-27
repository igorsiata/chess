#pragma once

#include <memory>
#include "game_logic/board_manager.hpp"
#include "game_logic/move_generator.hpp"
#include "game_logic/i_player.hpp"
#include <thread>


using PiecePositionChar = char[2];
using PieceChar = char;

class Game{
    public:
        Game() = default;
        Game(const std::string &positionFEN,
             std::unique_ptr<IPlayer> whitePlayer,
             std::unique_ptr<IPlayer> blackPlayer);
        void executeTurn();
        GameStatus getGameStatus() const {return m_gameStatus;}
        std::vector<std::pair<Position64, PieceChar>> getPiecesVec();
    private:
        BoardManager m_boardManager;
        GameStatus m_gameStatus;
        Color m_playerToMove;
        std::unique_ptr<IPlayer> m_whitePlayer;
        std::unique_ptr<IPlayer> m_blackPlayer;
        static const std::array<PieceChar, 12> pieceToCharMap;
        void updateGameStatus();
};

