#include "game_logic/legal_moves_generator.hpp"

std::vector<Move> LegalMovesGenerator::generateLegalMoves(const GameState &gameState)
{
    m_gameState = gameState;
    generateBasicMoves();
    return m_legalMoves;
}

void LegalMovesGenerator::generateBasicMoves()
{
    std::vector<Move> semiLegalMoves;
    for (const auto &positionAndPiece : m_gameState.piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != m_gameState.isWhiteToMove)
            continue;
        piecePtr->findPossibleMoves(m_gameState.piecesMap);
        std::vector<Move> newMoves = piecePtr->getPossibleMoves();
        semiLegalMoves.insert(semiLegalMoves.end(), newMoves.begin(), newMoves.end());
    }
}

Position LegalMovesGenerator::getKingPosition()
{
    for (const auto &positionAndPiece : m_gameState.piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() == m_gameState.isWhiteToMove && typeid(*piecePtr) == typeid(King))
            return positionAndPiece.first;
    }
    return Position(0, 0);
}

bool LegalMovesGenerator::isKingUnderAttack(const Position &kingPosition, const GameState &gameState)
{
    for (const auto &positionAndPiece : m_gameState.piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != m_gameState.isWhiteToMove)
        {
            piecePtr->findPossibleMoves(gameState.piecesMap);
            std::vector<Move> possibleMoves = piecePtr->getPossibleMoves();
        }
    }
}

bool LegalMovesGenerator::isCheckAfterMove(const Move &move)
{
    GameState dummyGameState = m_gameState;
    return false;
}

void LegalMovesGenerator::makeMove(const Move &move, GameState &gameState)
{
    if (move.moveType == CAPTURE)
    {
        gameState.piecesMap.erase(move.endPosition);
    }
    auto pieceHandler = gameState.piecesMap.extract(move.startPosition);
    pieceHandler.key() = move.endPosition;
    gameState.piecesMap.insert(std::move(pieceHandler));
}