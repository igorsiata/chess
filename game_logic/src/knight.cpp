#include "pieces/knight.hpp"

void Knight::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position currentPosition = getPosition();
    Position endPosition;
    int dx[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    int dy[8] = {1, -1, 2, -2, 2, -2, 1, -1};
    for (int i = 0; i < 8; i++)
    {
        endPosition = {currentPosition.x + dx[i],
                       currentPosition.y + dy[i]};
        moveType = calculateMoveType(endPosition, allPieces);
        if (moveType == EMPTY || moveType == CAPTURE)
        {
            possibleMoves.emplace_back(currentPosition, endPosition, moveType);
        }
    }
    m_possibleMoves = possibleMoves;
}