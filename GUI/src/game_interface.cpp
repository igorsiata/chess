#include "GUI/game_interface.hpp"

GameInterface::GameInterface(){
    const std::string startPostitionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"; 
    m_game.loadPosition(startPostitionFEN);
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