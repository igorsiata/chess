#include "game_logic/legal_moves_generator.hpp"

std::vector<Move> LegalMovesGenerator::generateLegalMoves(GameState &gameState)
{
    m_legalMoves.clear();
    PiecesMap piecesMap = gameState.getPiecesMap();
    for (const auto &positionAndPiece : piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() != gameState.isWhiteToMove())
            continue;
        piecePtr->findPossibleMoves(piecesMap);
        std::vector<Move> newMoves = piecePtr->getPossibleMoves();
        filterIllegallMoves(newMoves, gameState);
        m_legalMoves.insert(m_legalMoves.end(), newMoves.begin(), newMoves.end());
    }
    addCastles(gameState);
    addEnPassant(gameState);
    return m_legalMoves;
}
void LegalMovesGenerator::addCastles(GameState &gameState)
{
    addCastleKingside(gameState);
    addCastleQueenside(gameState);
}

void LegalMovesGenerator::addCastleKingside(GameState &gameState)
{
    Position kingPosition = getKingPosition(gameState);
    bool canCastleKingside = gameState.getAvaliableCastles()[gameState.isWhiteToMove()].castleKingside;
    if (!canCastleKingside)
        return;
    Position pos1 = kingPosition + Position(1, 0);
    Position pos2 = kingPosition + Position(2, 0);
    if (!isSquareEmpty(pos1, gameState) || !isSquareEmpty(pos2, gameState))
        return;
    if (isCheckAfterMove({kingPosition, kingPosition, EMPTY}, gameState) ||
        isCheckAfterMove({kingPosition, pos1, EMPTY}, gameState) ||
        isCheckAfterMove({kingPosition, pos2, EMPTY}, gameState))
        return;
    m_legalMoves.emplace_back(kingPosition, pos2, CASTLE_KINGSIDE);
}

void LegalMovesGenerator::addCastleQueenside(GameState &gameState)
{
    Position kingPosition = getKingPosition(gameState);
    bool canCastleQueenside = gameState.getAvaliableCastles()[gameState.isWhiteToMove()].castleQueenside;
    if (!canCastleQueenside)
        return;
    Position pos1 = kingPosition + Position(-1, 0);
    Position pos2 = kingPosition + Position(-2, 0);
    if (!isSquareEmpty(pos1, gameState) || !isSquareEmpty(pos2, gameState))
        return;
    if (isCheckAfterMove({kingPosition, kingPosition, EMPTY}, gameState) ||
        isCheckAfterMove({kingPosition, pos1, EMPTY}, gameState) ||
        isCheckAfterMove({kingPosition, pos2, EMPTY}, gameState))
        return;
    m_legalMoves.emplace_back(kingPosition, pos2, CASTLE_QUEENSIDE);
}

void LegalMovesGenerator::addEnPassant(GameState &gameState)
{
    if (!gameState.isEnPassantPossible())
        return;
    const Position &enPassantSquare = gameState.getEnPassantSquare();
    const int pawnRowDiff = gameState.isWhiteToMove() ? 1 : -1;
    const Position pawnPos1 = enPassantSquare + Position(-1, pawnRowDiff);
    const Position pawnPos2 = enPassantSquare + Position(1, pawnRowDiff);
    if (!isCheckAfterMove({pawnPos1, enPassantSquare, ENPASSANT}, gameState))
        m_legalMoves.emplace_back(pawnPos1, enPassantSquare, ENPASSANT);
    if (!isCheckAfterMove({pawnPos2, enPassantSquare, ENPASSANT}, gameState))
        m_legalMoves.emplace_back(pawnPos2, enPassantSquare, ENPASSANT);
}

void LegalMovesGenerator::filterIllegallMoves(std::vector<Move> &moves, GameState &gameState)
{
    moves.erase(
        std::remove_if(moves.begin(), moves.end(),
                       [this, &gameState](const Move &move)
                       { return isCheckAfterMove(move, gameState); }),
        moves.end());
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

bool LegalMovesGenerator::isKingUnderAttack(const Position &kingPosition, GameState &gameState)
{
    PiecesMap piecesMap = gameState.getPiecesMap();
    for (const auto &positionAndPiece : piecesMap)
    {
        const auto &piecePtr = positionAndPiece.second;
        if (piecePtr->isWhite() == gameState.isWhiteToMove())
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

bool LegalMovesGenerator::isCheckAfterMove(const Move &move, GameState &gameState)
{
    Position kingPosition = getKingPosition(gameState);
    if (move.startPosition == kingPosition)
        kingPosition = move.endPosition;
    gameState.movePiece(move);
    bool isCheck = isKingUnderAttack(kingPosition, gameState);
    gameState.reverseMovePiece(move);
    return isCheck;
}

bool LegalMovesGenerator::isSquareEmpty(const Position &position, const GameState &gameState)
{
    const PiecesMap &piecesMap = gameState.getPiecesMap();
    return piecesMap.find(position) == piecesMap.end();
}
