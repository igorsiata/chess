#include "game_logic/legal_moves_generator.hpp"

std::vector<Move> LegalMovesGenerator::generateLegalMoves(const GameState &gameState)
{
    m_legalMoves.clear();
    m_gameState = gameState;
    PiecesMap piecesMap = gameState.getPiecesMap();
    for (const auto &positionAndPiece : piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != gameState.isWhiteToMove())
            continue;
        piecePtr->findPossibleMoves(piecesMap);
        std::vector<Move> newMoves = piecePtr->getPossibleMoves();
        filterIllegallMoves(newMoves);
        m_legalMoves.insert(m_legalMoves.end(), newMoves.begin(), newMoves.end());
    }
    addCastles();
    addEnPassant();
    return m_legalMoves;
}
void LegalMovesGenerator::addCastles()
{
    addCastleKingside();
    addCastleQueenside();
}

void LegalMovesGenerator::addCastleQueenside()
{
    Position kingPosition = getKingPosition();
    bool canCastleKingside = m_gameState.getAvaliableCastles()[m_gameState.isWhiteToMove()].castleKingside;
    if (!canCastleKingside)
        return;
    Position pos1 = kingPosition + Position(1, 0);
    Position pos2 = kingPosition + Position(2, 0);
    if (!isSquareEmpty(pos1) || !isSquareEmpty(pos2))
        return;
    if (isCheckAfterMove({kingPosition, kingPosition, EMPTY}) ||
        isCheckAfterMove({kingPosition, pos1, EMPTY}) ||
        isCheckAfterMove({kingPosition, pos2, EMPTY}))
        return;
    m_legalMoves.emplace_back(kingPosition, pos2, CASTLE_KINGSIDE);
}

void LegalMovesGenerator::addCastleKingside()
{
    Position kingPosition = getKingPosition();
    bool canCastleQueenside = m_gameState.getAvaliableCastles()[m_gameState.isWhiteToMove()].castleQueenside;
    if (!canCastleQueenside)
        return;
    Position pos1 = kingPosition + Position(-1, 0);
    Position pos2 = kingPosition + Position(-2, 0);
    if (!isSquareEmpty(pos1) || !isSquareEmpty(pos2))
        return;
    if (isCheckAfterMove({kingPosition, kingPosition, EMPTY}) ||
        isCheckAfterMove({kingPosition, pos1, EMPTY}) ||
        isCheckAfterMove({kingPosition, pos2, EMPTY}))
        return;
    m_legalMoves.emplace_back(kingPosition, pos2, CASTLE_QUEENSIDE);
}

void LegalMovesGenerator::addEnPassant()
{
    if (!m_gameState.isEnPassantPossible())
        return;
    const Position &enPassantSquare = m_gameState.getEnPassantSquare();
    const int pawnRowDiff = m_gameState.isWhiteToMove() ? 1 : -1;
    const Position pawnPos1 = enPassantSquare + Position(-1, pawnRowDiff);
    const Position pawnPos2 = enPassantSquare + Position(1, pawnRowDiff);
    if (!isCheckAfterMove({pawnPos1, enPassantSquare, ENPASSANT}))
        m_legalMoves.emplace_back(pawnPos1, enPassantSquare, ENPASSANT);
    if (!isCheckAfterMove({pawnPos2, enPassantSquare, ENPASSANT}))
        m_legalMoves.emplace_back(pawnPos2, enPassantSquare, ENPASSANT);
}

void LegalMovesGenerator::filterIllegallMoves(std::vector<Move> &moves)
{
    moves.erase(
        std::remove_if(moves.begin(), moves.end(),
                       [this](const Move &move)
                       { return isCheckAfterMove(move); }),
        moves.end());
}

Position LegalMovesGenerator::getKingPosition()
{
    for (const auto &positionAndPiece : m_gameState.getPiecesMap())
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() == m_gameState.isWhiteToMove() && typeid(*piecePtr) == typeid(King))
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

bool LegalMovesGenerator::isCheckAfterMove(const Move &move)
{
    Position kingPosition = getKingPosition();
    if (move.startPosition == kingPosition)
        kingPosition = move.endPosition;
    GameState dummyGameState = m_gameState;
    dummyGameState.updatePiecesMap(move);
    return isKingUnderAttack(kingPosition, dummyGameState);
}

bool LegalMovesGenerator::isSquareEmpty(const Position &position)
{
    const PiecesMap &piecesMap = m_gameState.getPiecesMap();
    return piecesMap.find(position) == piecesMap.end();
}
