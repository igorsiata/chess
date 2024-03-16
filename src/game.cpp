#include "game.hpp"

void Game::move(const Position& startPosition, const Position& endPosition) {
    Piece* pieceToMove = m_piecesMap[startPosition];
    m_piecesMap.erase(startPosition);
    m_piecesMap[endPosition] = pieceToMove;
}

void Game::load_position(const std::string& positionAsString) {

    Position currentPosition(1, 1);
    for (auto it: positionAsString) {
        switch (it) {
            case ('b'):
                m_allPieces.emplace_back(Bishop(currentPosition, false));
                break;
            case ('B'):
                m_allPieces.emplace_back(Bishop(currentPosition, true));
                break;
            case 'P':
                m_allPieces.emplace_back(Pawn(currentPosition, true));
                break;
            case 'p':
                m_allPieces.emplace_back(Pawn(currentPosition, false));
                break;
            case 'R':
                m_allPieces.emplace_back(Rook(currentPosition, true));
                break;
            case 'r':
                m_allPieces.emplace_back(Rook(currentPosition, false));
                break;
            case 'N':
                m_allPieces.emplace_back(Kinght(currentPosition, true));
                break;
            case 'n':
                m_allPieces.emplace_back(Kinght(currentPosition, false));
                break;
            case 'Q':
                m_allPieces.emplace_back(Queen(currentPosition, true));
                break;
            case 'q':
                m_allPieces.emplace_back(Queen(currentPosition, false));
                break;
            case 'K':
                m_allPieces.emplace_back(King(currentPosition, true));
                break;
            case 'k':
                m_allPieces.emplace_back(King(currentPosition, false));
                break;
            case ('/'):
                currentPosition.second += 1;
                currentPosition.first = 0;
                break;
            default:
                if (std::isdigit(it)) {
                    currentPosition.first += uint8_t(it - 48) - 1;
                }
                break;
        }
        currentPosition.first++;
    }
    for (auto it = m_allPieces.begin(); it != m_allPieces.end(); it++) {
        m_piecesMap[it->get_position()] = &*it;
    }
}

void Game::generate_position(std::ostream& os) {
    std::string horizontalLine = "\n+---+---+---+---+---+---+---+---+\n";
    for (std::size_t row = 1; row <= 8; row++) {
        os << horizontalLine;
        os << "|";
        for (std::size_t column = 1; column <= 8; column++) {
            auto it = m_piecesMap.find(Position(column, row));
            if (it != m_piecesMap.end())
                os << " " << it->second->get_id();
            else
                os << "  ";
            os << " |";
        }
    }
    os << horizontalLine;
}

void Game::generate_possible_moves() {
    for (auto it = m_allPieces.begin(); it != m_allPieces.end(); it++) {
        it->find_possible_moves(m_piecesMap);
    }
}








