#include "game_logic/game.hpp"

void Game::makeMove(std::shared_ptr<Piece> piece, const Move move){
    m_gameState.updateGameState(move);
}

void Game::loadPositionFEN(const std::string &positionFEN){
    m_gameState.loadGameStateFromFEN(positionFEN);
}

std::vector<Move> Game::getPossibleMoves(){
    LegalMovesGenerator legalMovesGenerator;
    return legalMovesGenerator.generateLegalMoves(m_gameState);
}
