#include "GUI/game_interface.hpp"

GameInterface::GameInterface(){
    const std::string startPostitionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"; 
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

std::vector<Move> GameInterface::getPiecePossibleMoves(const Position &piecePosition){
    std::vector<Move> allMoves = m_game.getPossibleMoves();
    allMoves.erase(
        std::remove_if(allMoves.begin(), allMoves.end(),
            [piecePosition](const Move &move)
             { return !(move.startPosition == piecePosition); }),
             allMoves.end()
    );
    return allMoves;
}