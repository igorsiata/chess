#include "game_logic/king.hpp"

void King::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    int dx[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    int dy[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
    for (int i = 0; i < 9; i++)
    {
        endPosition = {getPosition().x + dx[i],
                       getPosition().y + dy[i]};
        moveType = calculateMoveType(isWhite(), endPosition, allPieces);
        if (moveType == EMPTY)
        {
            possibleMoves.emplace_back(Move{endPosition, moveType});
        }
        if (moveType == CAPTURE)
        {
            possibleMoves.emplace_back(Move{endPosition, moveType});
        }
    }
    m_possibleMoves = possibleMoves;
}