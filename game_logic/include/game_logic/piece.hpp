#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include "move.hpp"

class Piece;
using PiecesMap = std::map<Position, std::shared_ptr<Piece>>;

class Piece
{
public:
    Piece(const Position position, bool isWhite) : m_position(position), m_isWhite(isWhite) {};
    virtual void findPossibleMoves(const PiecesMap &allPieces){};
    const std::vector<Move> getPossibleMoves() const {return m_possibleMoves;};
    const Position getPosition() const { return m_position; };
    const bool isWhite() const { return m_isWhite; };
    const void move(const Position endPostion) { m_position = endPostion; };
protected:
    std::vector<Move> m_possibleMoves;
private:
    Position m_position;
    bool m_isWhite;
};

