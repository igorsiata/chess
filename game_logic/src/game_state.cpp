#include "game_logic/game_state.hpp"

void GameState::loadGameStateFromFEN(const std::string &positionFEN)
{
    loadPieces(positionFEN);
    loadPlayerToMove(positionFEN);
    loadAvailableCastles(positionFEN);
    loadEnPassantSquare(positionFEN);
}

void GameState::movePiece(const Move &move)
{
    const auto piecesMapIt = m_piecesMap.find(move.startPosition);
    if (piecesMapIt != m_piecesMap.end())
    {
        const auto piecePtr = piecesMapIt->second;
        piecePtr->move(move.endPosition);
    }
    m_gameStateBackup = std::make_shared<GameState>(*this);
    updatePiecesMap(move);
    updateAvaliableCastles(move);
    if (move.moveType == CASTLE_QUEENSIDE){
        const Position rookStartPosition = move.endPosition + Position(-2, 0);
        const Position rookEndPosition = move.endPosition + Position(1, 0);
        const Move rookMove(rookStartPosition, rookEndPosition, EMPTY);
        movePiece(rookMove);
        return;
    }
    if (move.moveType == CASTLE_KINGSIDE){
        const Position rookStartPosition = move.endPosition + Position(1, 0);
        const Position rookEndPosition = move.endPosition + Position(-1, 0);
        const Move rookMove(rookStartPosition, rookEndPosition, EMPTY);
        movePiece(rookMove);
        return;
    }
    m_isWhiteToMove = !m_isWhiteToMove;
}

void GameState::reverseMovePiece(const Move &move)
{
    const auto piecesMapIt = m_piecesMap.find(move.endPosition);
    if (piecesMapIt != m_piecesMap.end())
    {
        const auto piecePtr = piecesMapIt->second;
        piecePtr->move(move.startPosition);
    }
    if (m_gameStateBackup){
        m_piecesMap = m_gameStateBackup->getPiecesMap();
        m_availableCastles = m_gameStateBackup->getAvaliableCastles();
        m_enPassantSquare = m_gameStateBackup->getEnPassantSquare();
        m_isWhiteToMove = m_gameStateBackup->isWhiteToMove();
    }

}

void GameState::updatePiecesMap(const Move &move)
{
    // Doesn't change piece internal position, only its square in map
    if (move.moveType == CAPTURE)
    {
        m_piecesMap.erase(move.endPosition);
    }
    auto pieceToMove = m_piecesMap[move.startPosition];
    auto pieceHandler = m_piecesMap.extract(move.startPosition);
    pieceHandler.key() = move.endPosition;
    m_piecesMap.insert(std::move(pieceHandler));
}

void GameState::updateAvaliableCastles(const Move &move)
{
    const int kingRow = m_isWhiteToMove ? 8 : 1;
    bool isKingMoved = move.startPosition == Position(5, kingRow);
    bool isKingRookMoved = move.startPosition == Position(8, kingRow);
    bool isQueenRookMoved = move.startPosition == Position(1, kingRow);
    if (isKingMoved || isKingRookMoved)
    {
        m_availableCastles[m_isWhiteToMove].castleKingside = false;
    }
    if (isKingMoved || isQueenRookMoved)
    {
        m_availableCastles[m_isWhiteToMove].castleQueenside = false;
    }

    const int enemyKingRow = m_isWhiteToMove ? 1 : 8;
    bool isKingRookCaptured = move.endPosition == Position(8, enemyKingRow);
    bool isQueenRookCaptured = move.endPosition == Position(1, enemyKingRow);
    if (isKingRookCaptured)
        m_availableCastles[!m_isWhiteToMove].castleKingside = false;
    if (isQueenRookCaptured)
        m_availableCastles[!m_isWhiteToMove].castleQueenside = false;
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
    size_t firstSpaceIdx = positionFEN.find(' ');
    size_t secondSpaceIdx = positionFEN.find(' ', firstSpaceIdx + 1);
    if (firstSpaceIdx == std::string::npos || secondSpaceIdx == std::string::npos)
    {
        return;
    }
    for (auto it : positionFEN.substr(secondSpaceIdx + 1))
    {
        if (it == ' ')
            return;
        if (it == 'K')
            m_availableCastles[true].castleKingside = true;

        if (it == 'Q')
            m_availableCastles[true].castleQueenside = true;

        if (it == 'k')
            m_availableCastles[false].castleKingside = true;

        if (it == 'q')
            m_availableCastles[false].castleQueenside = true;
    }
}

void GameState::loadEnPassantSquare(const std::string &positionFEN)
{
    size_t firstSpaceIdx = positionFEN.find(' ');
    size_t secondSpaceIdx = positionFEN.find(' ', firstSpaceIdx + 1);
    size_t thirdSpaceIdx = positionFEN.find(' ', secondSpaceIdx + 1);
    if (firstSpaceIdx == std::string::npos || secondSpaceIdx == std::string::npos || thirdSpaceIdx == std::string::npos)
    {
        return;
    }
    auto subStr = positionFEN.substr(thirdSpaceIdx + 1);
    if (subStr[0] == '-')
    {
        m_enPassantSquare = Position(0, 0);
    }
    else
    {
        m_enPassantSquare.x = subStr[0] - 96;
        m_enPassantSquare.y = 9 - (subStr[1] - 48);
    }
}
