#include "game_logic/game.hpp"

void Game::makeMove(std::shared_ptr<Piece> piece, const Move move)
{
    if (move.moveType == CAPTURE)
    {
        m_gameState.piecesMap.erase(move.endPosition);
    }
    Position currentPosition = piece->getPosition();
    piece->move(move.endPosition);
    auto pieceHandler = m_gameState.piecesMap.extract(currentPosition);
    pieceHandler.key() = move.endPosition;
    m_gameState.piecesMap.insert(std::move(pieceHandler));
}

void Game::loadPosition(const std::string &positionFEN){
    PositionLoader positionLoader;
    m_gameState = positionLoader.loadPositionFromFEN(positionFEN);
}
// void Game::calculatePossibleMovesForAllPieces()

// {
//     for (const auto &positionAndPiece : m_allPieces)
//     {
//         const auto &piecePtr = positionAndPiece.second;
//         piecePtr->findPossibleMoves(m_allPieces);
//         std::vector<Move> possibleMoves = piecePtr->getPossibleMoves();
//         for (Move move : possibleMoves)
//         {
//             if (isCheckAfterMove(piecePtr, move))
//             {
//                 // Remove the move
//             }
//         }
//     }
// }

// bool Game::isCheckAfterMove(const std::shared_ptr<Piece> piece, Move move)
// {
//     return false;
// }

