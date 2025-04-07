#include "pieces/piece.hpp"

MoveType Piece::calculateMoveType(const Position move,
                           const PiecesMap &allPieces)
{
    // is move out of board
    if (move.x < 1 || move.x > 8 || move.y < 1 || move.y > 8)
        return MoveType::OUT;

    // empty square
    auto pieceAtMovePosition = allPieces.find(move);
    if (pieceAtMovePosition == allPieces.end())
    {
        return MoveType::EMPTY;
    }
    // is move colliding with any other piece
    if (pieceAtMovePosition->second->isWhite() == m_isWhite)
    {
        return MoveType::ALLY;
    }
    else
    {
        return MoveType::CAPTURE;
    }
}