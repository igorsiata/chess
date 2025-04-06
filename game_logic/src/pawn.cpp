#include "game_logic/pawn.hpp"

void Pawn::findPossibleMoves(const PiecesMap &allPieces)
{
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int colorMult = 1;
    if (isWhite())
    {
        colorMult = -1;
    }

    // move 2 squares
    if ((isWhite() && getPosition().y == 7) || (!isWhite() && getPosition().y == 2))
    {
        endPosition = {
            getPosition().x,
            getPosition().y + 2 * colorMult};
        moveType = calculateMoveType(isWhite(), endPosition, allPieces);
        move = Move{endPosition, moveType};
        if (moveType == EMPTY)
        {
            possibleMoves.push_back(move);
        }
    }

    // move 1 square
    endPosition = {
        getPosition().x,
        getPosition().y + 1 * colorMult};
    moveType = calculateMoveType(isWhite(), endPosition, allPieces);
    move = Move{endPosition, moveType};
    if (moveType == EMPTY)
    {
        possibleMoves.push_back(move);
    }
    else if (!possibleMoves.empty())
    {
        possibleMoves.pop_back();
    }

    // captures
    for (int i = -1; i <= 1; i += 2)
    {
        endPosition = {
            getPosition().x + i,
            getPosition().y + 1 * colorMult};
        moveType = calculateMoveType(isWhite(), endPosition, allPieces);
        move = Move{endPosition, moveType};
        if (moveType == CAPTURE)
        {
            possibleMoves.push_back(move);
        }
    }
    m_possibleMoves = possibleMoves;
}