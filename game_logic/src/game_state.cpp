// #include "game_logic/game_state.hpp"


// void GameStateUpdater::loadGameStateFromStandardNotation(const std::string &standardNotation)
// {
//     PositionLoader positionLoader;
//     positionLoader.loadPositionFromStandardNotation(standardNotation);
// }
// void GameStateUpdater::updateGameState(const Move &move)
// {
//     if (move.moveType == CAPTURE)
//     {
//         m_piecesMap.erase(move.endPosition);
//     }
//     auto pieceToMove = m_piecesMap[move.startPosition];
//     pieceToMove->move(move.endPosition);
//     auto pieceHandler = m_piecesMap.extract(move.startPosition);
//     pieceHandler.key() = move.endPosition;
//     m_piecesMap.insert(std::move(pieceHandler));
// }