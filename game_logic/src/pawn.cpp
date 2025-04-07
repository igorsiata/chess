#include "pieces/pawn.hpp"

void Pawn::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Position currentPosition = getPosition();
    int colorMult = 1;
    if (isWhite())
    {
        colorMult = -1;
    }

    // move 2 squares
    if ((isWhite() && currentPosition.y == 7) || (!isWhite() && currentPosition.y == 2))
    {
        endPosition = {currentPosition.x,
                       currentPosition.y + 2 * colorMult};
        moveType = calculateMoveType(endPosition, allPieces);
        if (moveType == EMPTY)
        {
            possibleMoves.emplace_back(currentPosition, endPosition, moveType);
        }
    }

    // move 1 square
    endPosition = {currentPosition.x,
                   currentPosition.y + 1 * colorMult};
    moveType = calculateMoveType(endPosition, allPieces);
    if (moveType == EMPTY)
    {
        possibleMoves.emplace_back(currentPosition, endPosition, moveType);
    }
    else if (!possibleMoves.empty())
    {
        possibleMoves.pop_back();
    }

    // captures
    for (int i = -1; i <= 1; i += 2)
    {
        endPosition = {currentPosition.x + i,
                       currentPosition.y + 1 * colorMult};
        moveType = calculateMoveType(endPosition, allPieces);
        if (moveType == CAPTURE)
        {
            possibleMoves.emplace_back(currentPosition, endPosition, moveType);
        }
    }
    m_possibleMoves = possibleMoves;
}