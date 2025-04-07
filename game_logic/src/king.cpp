#include "pieces/king.hpp"

void King::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Position currentPosition = getPosition();
    int dx[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    int dy[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
    for (int i = 0; i < 9; i++)
    {
        endPosition = {currentPosition.x + dx[i],
                       currentPosition.y + dy[i]};
        moveType = calculateMoveType(endPosition, allPieces);
        if (moveType == EMPTY)
        {
            possibleMoves.emplace_back(currentPosition, endPosition, moveType);
        }
        if (moveType == CAPTURE)
        {
            possibleMoves.emplace_back(currentPosition, endPosition, moveType);
        }
    }
    m_possibleMoves = possibleMoves;
}