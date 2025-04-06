#include "game_logic/bishop.hpp"

void Bishop::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    int dx[4] = {1, 1, -1, -1};
    int dy[4] = {1, -1, 1, -1};
    for (int i = 0; i < 4; i++)
    {

        for (int step = 1; step <= 8; step++)
        {
            endPosition = {getPosition().x + dx[i] * step,
                           getPosition().y + dy[i] * step};
            moveType = calculateMoveType(isWhite(), endPosition, allPieces);
            if (moveType == EMPTY)
            {
                possibleMoves.emplace_back(Move{endPosition, moveType});
            }
            if (moveType == OUT || moveType == ALLY)
            {
                break;
            }
            if (moveType == CAPTURE)
            {
                possibleMoves.emplace_back(Move{endPosition, moveType});
                break;
            }
        }
    }
    m_possibleMoves = possibleMoves;
}