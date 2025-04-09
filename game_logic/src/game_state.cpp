#include "game_logic/game_state.hpp"

void GameState::loadGameStateFromFEN(const std::string &positionFEN)
{
    
    loadPieces(positionFEN);
    loadPlayerToMove(positionFEN);
}
void GameState::updateGameState(const Move &move)
{
    // Doesn't change piece internal position, only its square in map
    if (move.moveType == CAPTURE)
    {
        m_piecesMap.erase(move.endPosition);
    }
    auto pieceToMove = m_piecesMap[move.startPosition];
    //pieceToMove->move(move.endPosition);
    auto pieceHandler = m_piecesMap.extract(move.startPosition);
    pieceHandler.key() = move.endPosition;
    m_piecesMap.insert(std::move(pieceHandler));
}

void GameState::movePiece(const Move &move){
    const auto piecesMapIt = m_piecesMap.find(move.startPosition);
    if (piecesMapIt != m_piecesMap.end()) {
        const auto piecePtr = piecesMapIt->second;
        piecePtr->move(move.endPosition);
    }
    updateGameState(move);
    
}

void GameState::loadPieces(const std::string &positionFEN)
{
    m_piecesMap.clear();
    Position currentPosition(1, 1);
    for (auto it : positionFEN)
    {
        switch (it)
        {
        case 'b':
            m_piecesMap.insert({currentPosition, std::make_shared<Bishop>(currentPosition, false)});
            break;
        case 'B':
            m_piecesMap.insert({currentPosition, std::make_shared<Bishop>(currentPosition, true)});
            break;
        case 'P':
            m_piecesMap.insert({currentPosition, std::make_shared<Pawn>(currentPosition, true)});
            break;
        case 'p':
            m_piecesMap.insert({currentPosition, std::make_shared<Pawn>(currentPosition, false)});
            break;
        case 'R':
            m_piecesMap.insert({currentPosition, std::make_shared<Rook>(currentPosition, true)});
            break;
        case 'r':
            m_piecesMap.insert({currentPosition, std::make_shared<Rook>(currentPosition, false)});
            break;
        case 'N':
            m_piecesMap.insert({currentPosition, std::make_shared<Knight>(currentPosition, true)});
            break;
        case 'n':
            m_piecesMap.insert({currentPosition, std::make_shared<Knight>(currentPosition, false)});
            break;
        case 'Q':
            m_piecesMap.insert({currentPosition, std::make_shared<Queen>(currentPosition, true)});
            break;
        case 'q':
            m_piecesMap.insert({currentPosition, std::make_shared<Queen>(currentPosition, false)});
            break;
        case 'K':
            m_piecesMap.insert({currentPosition, std::make_shared<King>(currentPosition, true)});
            break;
        case 'k':
            m_piecesMap.insert({currentPosition, std::make_shared<King>(currentPosition, false)});
            break;
        case '/':
            currentPosition.y += 1;
            currentPosition.x = 0;
            break;
        case ' ':
            return;
        default:
            if (std::isdigit(it))
            {
                currentPosition.x += uint8_t(it - 48) - 1;
            }
            break;
        }
        currentPosition.x++;
    }
}

void GameState::loadPlayerToMove(const std::string &positionFEN)
{
    int firstSpaceIdx = positionFEN.find(' ');
    char playerToMove = positionFEN[firstSpaceIdx + 1];
    if (playerToMove == 'w')
    {
        m_isWhiteToMove = true;
    }
    else if (playerToMove == 'b')
    {
        m_isWhiteToMove = false;
    }
}

void GameState::loadAvailableCastles(const std::string &positionFEN)
{
    // TODO
    char it = 'q';
    std::map<char, bool> m_CastlesAvailable = {{'K', false},
                                               {'Q', false},
                                               {'k', false},
                                               {'q', false}};
    m_CastlesAvailable[it] = true;
}
