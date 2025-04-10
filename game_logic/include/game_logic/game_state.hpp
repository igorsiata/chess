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

struct AvailableCastles
{
    bool castleQueenside = false;
    bool castleKingside = false;
};

class GameState
{
public:
    void loadGameStateFromFEN(const std::string &positionFEN);
    void updatePiecesMap(const Move &move);
    void movePiece(const Move &move);
    void reverseMovePiece(const Move &move);

    PiecesMap getPiecesMap() const { return m_piecesMap; }
    bool isWhiteToMove() const { return m_isWhiteToMove; }
    std::map<bool, AvailableCastles> getAvaliableCastles() const { return m_availableCastles; }
    bool isEnPassantPossible() const {return !(m_enPassantSquare == Position(0,0));}
    Position getEnPassantSquare() const {return m_enPassantSquare;}
    ~GameState() = default;
private:
    PiecesMap m_piecesMap;
    bool m_isWhiteToMove;
    std::map<bool, AvailableCastles> m_availableCastles = {{true, AvailableCastles{}}, {false, AvailableCastles{}}};
    Position m_enPassantSquare = Position(0,0);
    std::shared_ptr<Piece> m_lastCapturedPiece;
    std::shared_ptr<GameState> m_gameStateBackup;

    void updateAvaliableCastles(const Move &move);
    void loadPieces(const std::string &standardNotation);
    void loadPlayerToMove(const std::string &standardNotation);
    void loadAvailableCastles(const std::string &standardNotation);
    void loadEnPassantSquare(const std::string &positionFEN);
};
