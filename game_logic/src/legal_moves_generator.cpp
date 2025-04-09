#include "game_logic/legal_moves_generator.hpp"

std::vector<Move> LegalMovesGenerator::generateLegalMoves(const GameState &gameState)
{
    m_gameState = gameState;
    std::vector<Move> legalMoves = generateLegalMoves();
    return legalMoves;
}

std::vector<Move> LegalMovesGenerator::generateLegalMoves()
{
    std::vector<Move> legalMoves;
    for (const auto &positionAndPiece : m_gameState.piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != m_gameState.isWhiteToMove)
            continue;
        piecePtr->findPossibleMoves(m_gameState.piecesMap);
        std::vector<Move> newMoves = piecePtr->getPossibleMoves();
        filterIllegallMoves(newMoves);
        legalMoves.insert(legalMoves.end(), newMoves.begin(), newMoves.end());
    }
    return legalMoves;
}

void LegalMovesGenerator::filterIllegallMoves(std::vector<Move> &moves) {
    moves.erase(
        std::remove_if(moves.begin(), moves.end(),
            [this](const Move &move) { return isCheckAfterMove(move); }),
            moves.end()
    );
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
    for (const auto &positionAndPiece : gameState.piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != m_gameState.isWhiteToMove)
        {
            piecePtr->findPossibleMoves(gameState.piecesMap);
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

bool LegalMovesGenerator::isCheckAfterMove(const Move &move)
{
    Position kingPosition = getKingPosition();
    if (move.startPosition == kingPosition)
        kingPosition = move.endPosition;
    GameState dummyGameState = m_gameState;
    makeMove(move, dummyGameState);
    return isKingUnderAttack(kingPosition, dummyGameState);
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