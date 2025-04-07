#include "game_logic/position_loader.hpp"

GameState PositionLoader::loadPositionFromFEN(const std::string &positionFEN)
{
    loadPieces(positionFEN);
    loadPlayerToMove(positionFEN);
    // loadAvailableCastles(positionFEN);
    return m_gameState;
}

void PositionLoader::loadPieces(const std::string &positionFEN)
{
    Position currentPosition = {1, 1};
    for (auto it : positionFEN)
    {
        switch (it)
        {
        case 'b':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Bishop>(currentPosition, false)});
            break;
        case 'B':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Bishop>(currentPosition, true)});
            break;
        case 'P':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Pawn>(currentPosition, true)});
            break;
        case 'p':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Pawn>(currentPosition, false)});
            break;
        case 'R':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Rook>(currentPosition, true)});
            break;
        case 'r':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Rook>(currentPosition, false)});
            break;
        case 'N':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Knight>(currentPosition, true)});
            break;
        case 'n':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Knight>(currentPosition, false)});
            break;
        case 'Q':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Queen>(currentPosition, true)});
            break;
        case 'q':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<Queen>(currentPosition, false)});
            break;
        case 'K':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<King>(currentPosition, true)});
            break;
        case 'k':
            m_gameState.piecesMap.insert({currentPosition, std::make_shared<King>(currentPosition, false)});
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

void PositionLoader::loadPlayerToMove(const std::string &positionFEN)
{
    int firstSpaceIdx = positionFEN.find(' ');
    char playerToMove = positionFEN[firstSpaceIdx + 1];
    if (playerToMove == 'w')
    {
        m_gameState.isWhiteToMove = true;
    }
    else if (playerToMove == 'b')
    {
        m_gameState.isWhiteToMove = false;
    }
}

void PositionLoader::loadAvailableCastles(const std::string &positionFEN)
{
    // TODO
    char it = 'q';
    std::map<char, bool> m_CastlesAvailable = {{'K', false},
                                               {'Q', false},
                                               {'k', false},
                                               {'q', false}};
    m_CastlesAvailable[it] = true;
}

