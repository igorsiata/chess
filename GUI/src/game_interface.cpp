#include "GUI/game_interface.hpp"

GameInterface::GameInterface()
{
    const std::string startPostitionFEN = "r3k2r/pppppppp/8/8/2r5/8/PP1PPPPP/R3KB1R w KQkq -";
    m_game.loadPositionFEN(startPostitionFEN);
}

std::map<Position, char> GameInterface::getPiecesAsChar() const
{
    std::map<Position, char> piecesAsChar;
    PiecesMap piecesMap = m_game.getPiecesMap();
    for (auto &pair : piecesMap)
    {
        piecesAsChar[pair.first] = pair.second->getCharRepresentation();
    }
    return piecesAsChar;
}

std::vector<Move> GameInterface::getPiecePossibleMoves(const Position &piecePosition)
{
    std::vector<Move> allMoves = m_game.getPossibleMoves();
    allMoves.erase(
        std::remove_if(allMoves.begin(), allMoves.end(),
                       [piecePosition](const Move &move)
                       { return !(move.startPosition == piecePosition); }),
        allMoves.end());
    return allMoves;
}

void GameInterface::movePiece(const Position &startSquare, const Position &endSquare)
{
    const auto &possibleMoves = m_game.getPossibleMoves();
    for (const auto &move : possibleMoves){
        if (move.startPosition == startSquare and move.endPosition==endSquare){
            m_game.makeMove(move);
            return;
        }
    }
}