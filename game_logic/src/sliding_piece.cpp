#include "pieces/sliding_piece.hpp"

void SlidingPiece::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Position currentPosition = getPosition();
    for (int i = 0; i < m_numberOfDirections; i++)
    {

        for (int step = 1; step <= 8; step++)
        {
            endPosition = {currentPosition.x + m_directionX[i] * step,
                           currentPosition.y + m_directionY[i] * step};
            moveType = calculateMoveType(endPosition, allPieces);
            if (moveType == EMPTY)
            {
                possibleMoves.emplace_back(currentPosition, endPosition, moveType);
            }
            if (moveType == OUT || moveType == ALLY)
            {
                break;
            }
            if (moveType == CAPTURE)
            {
                possibleMoves.emplace_back(currentPosition, endPosition, moveType);
                break;
            }
        }
    }
    m_possibleMoves = possibleMoves;
}