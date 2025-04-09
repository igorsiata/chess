#pragma once
#include <vector>
#include <typeinfo>
#include "pieces/piece.hpp"
#include "pieces/bishop.hpp"
#include "pieces/knight.hpp"
#include "pieces/pawn.hpp"
#include "pieces/rook.hpp"
#include "pieces/queen.hpp"
#include "pieces/king.hpp"
#include <map>
#include <string>

class GameState
{
public:
    void loadGameStateFromFEN(const std::string &positionFEN);
    void updateGameState(const Move &move);
    void movePiece(const Move &move);

    PiecesMap getPiecesMap() const { return m_piecesMap; }
    bool isWhiteToMove() const { return m_isWhiteToMove; }
    std::vector<Position> getAvaliableCastles() const { return m_availableCastles; }

private:
    PiecesMap m_piecesMap;
    bool m_isWhiteToMove;
    std::vector<Position> m_availableCastles;
    bool m_isEnPassantPossible;
    Position m_enPassantSquare;

    void loadPieces(const std::string &standardNotation);
    void loadPlayerToMove(const std::string &standardNotation);
    void loadAvailableCastles(const std::string &standardNotation);
};
