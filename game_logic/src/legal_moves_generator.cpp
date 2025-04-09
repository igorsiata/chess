#include "game_logic/legal_moves_generator.hpp"


std::vector<Move> LegalMovesGenerator::generateLegalMoves(const GameState &gameState)
{
    std::vector<Move> legalMoves;
    PiecesMap piecesMap = gameState.getPiecesMap();
    for (const auto &positionAndPiece : piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != gameState.isWhiteToMove())
            continue;
        piecePtr->findPossibleMoves(piecesMap);
        std::vector<Move> newMoves = piecePtr->getPossibleMoves();
        filterIllegallMoves(newMoves, gameState);
        legalMoves.insert(legalMoves.end(), newMoves.begin(), newMoves.end());
    }
    return legalMoves;
}

void LegalMovesGenerator::filterIllegallMoves(std::vector<Move> &moves, const GameState &gameState) {
    moves.erase(
        std::remove_if(moves.begin(), moves.end(),
            [this, gameState](const Move &move)
             { return isCheckAfterMove(move, gameState); }),
            moves.end()
    );
}

Position LegalMovesGenerator::getKingPosition(const GameState &gameState)
{
    for (const auto &positionAndPiece : gameState.getPiecesMap())
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() == gameState.isWhiteToMove() && typeid(*piecePtr) == typeid(King))
            return positionAndPiece.first;
    }
    return Position(0, 0);
}

bool LegalMovesGenerator::isKingUnderAttack(const Position &kingPosition, const GameState &gameState)
{
    PiecesMap piecesMap = gameState.getPiecesMap();
    for (const auto &positionAndPiece : piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != gameState.isWhiteToMove())
        {
            piecePtr->findPossibleMoves(piecesMap);
            std::vector<Move> possibleMoves = piecePtr->getPossibleMoves();
            auto it = std::find_if(possibleMoves.begin(),
                                   possibleMoves.end(),
                                   [&kingPosition](const Move &elem)
                                   { return elem.endPosition == kingPosition; });
            if (it != possibleMoves.end())
                return true;
        }
    }
    return false;
}

bool LegalMovesGenerator::isCheckAfterMove(const Move &move, const GameState &gameState)
{
    Position kingPosition = getKingPosition(gameState);
    if (move.startPosition == kingPosition)
        kingPosition = move.endPosition;
    GameState dummyGameState = gameState;
    dummyGameState.updateGameState(move);
    return isKingUnderAttack(kingPosition, dummyGameState);
}

