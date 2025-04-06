#include "game_logic/knight.hpp"

void Knight::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int dx[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    int dy[8] = {1, -1, 2, -2, 2, -2, 1, -1};
    for (int i = 0; i < 8; i++)
    {
        endPosition = {
            getPosition().x + dx[i],
            getPosition().y + dy[i]};
        moveType = calculateMoveType(isWhite(), endPosition, allPieces);
        move = Move{endPosition, moveType};
        switch (moveType)
        {
        case OUT:
            break;
        case EMPTY:
            possibleMoves.push_back(move);
            break;
        case CAPTURE:
            possibleMoves.push_back(move);
            break;
        case ALLY:
            break;
        case CHECK:
            possibleMoves.push_back(move);
            break;
        }
    }
    m_possibleMoves = possibleMoves;
}