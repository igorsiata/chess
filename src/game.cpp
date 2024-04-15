#include "game.hpp"

void Game::move(const Move& move) {
    if (move.moveType == CAPTURE) {
        m_piecesMap.erase(move.endPosition);
    }
    if (move.moveType == EMPTY or move.moveType == CAPTURE) {
        Piece* pieceToMove = m_piecesMap[move.startPosition];
        m_piecesMap[move.endPosition] = &*pieceToMove;
        m_piecesMap.erase(move.startPosition);
    }


}

void Game::load_position(const std::string& positionAsString) {

    Position currentPosition(1, 1);
    for (auto it: positionAsString) {
        switch (it) {
            case ('b'):
                m_piecesMap.insert({currentPosition, new Bishop(currentPosition, false)});
                break;
            case ('B'):
                m_piecesMap.insert({currentPosition, new Bishop(currentPosition, true)});
                break;
            case 'P':
                m_piecesMap.insert({currentPosition, new Pawn(currentPosition, true)});
                break;
            case 'p':
                m_piecesMap.insert({currentPosition, new Pawn(currentPosition, false)});
                break;
            case 'R':
                m_piecesMap.insert({currentPosition, new Rook(currentPosition, true)});
                break;
            case 'r':
                m_piecesMap.insert({currentPosition, new Rook(currentPosition, false)});
                break;
            case 'N':
                m_piecesMap.insert({currentPosition, new Kinght(currentPosition, true)});
                break;
            case 'n':
                m_piecesMap.insert({currentPosition, new Kinght(currentPosition, false)});
                break;
            case 'Q':
                m_piecesMap.insert({currentPosition, new Queen(currentPosition, true)});
                break;
            case 'q':
                m_piecesMap.insert({currentPosition, new Queen(currentPosition, false)});
                break;
            case 'K':
                m_piecesMap.insert({currentPosition, new King(currentPosition, true)});
                break;
            case 'k':
                m_piecesMap.insert({currentPosition, new King(currentPosition, false)});
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
    for (auto& it: m_piecesMap) {
            it.second->find_possible_moves(m_piecesMap);
    }
    //add castles
    std::vector<Move> castles = find_castles();
    if (!castles.empty()) {
        int y = 1 + int(m_whiteToMove) * 7;
        auto kingItr = m_piecesMap.find(Position(5, y));
        kingItr->second->add_possible_moves(castles);
    }

    //add en passant square
}

void Game::output_possible_moves(std::ostream& os) {
    std::string horizontalLine = "\n-----------------------+\n";
    for (auto& itr: m_piecesMap) {
        os << horizontalLine;
        os << itr.second->get_id() << " (" << itr.second->get_position().first <<
           ", " << itr.second->get_position().second << ") -> \n";
        auto piece = *(itr.second);
        for (auto move: piece.get_possible_moves()) {
            os << "(" << move.endPosition.first << ", " << move.endPosition.second << "); ";
        }
    }
}

Game::Game() : m_whiteToMove(true), m_enPassantSquare(0, 0) {
    m_CastlesAvailable = {{'K', true},
                          {'Q', true},
                          {'k', true},
                          {'q', true}};
}


std::vector<Move> Game::find_castles() {
    std::vector<Move> castles;
    if (m_whiteToMove and (!m_CastlesAvailable['K'] and !m_CastlesAvailable['Q'])) {
        return castles;
    }
    // y -> row where king stands
    int y = 1 + int(m_whiteToMove) * 7;

    auto kingItr = m_piecesMap.find(Position(5, y));
    if (kingItr == m_piecesMap.end()) {
        if (m_whiteToMove) {
            m_CastlesAvailable['K'] = false;
            m_CastlesAvailable['Q'] = false;
        } else {
            m_CastlesAvailable['k'] = false;
            m_CastlesAvailable['q'] = false;
        }
        return castles;
    }
    bool canKingside = true;
    bool canQueeside = true;

    //squares to check if they are empty and not under attack
    int kingside[2] = {6, 7};
    int queenside[3] = {4, 3, 2};

    //empty kingside
    for (int i = 0; i < 2; i++) {
        if (m_piecesMap.find(Position(kingside[i], y)) != m_piecesMap.end()) {
            canKingside = false;
        }
    }

    //empty queenside
    for (int i = 0; i < 3; i++) {
        if (m_piecesMap.find(Position(queenside[i], y)) != m_piecesMap.end()) {
            canKingside = false;
        }
    }

    //check if squares are under attack
    for (auto& elem: m_piecesMap) {
        Piece* enemyPiece = elem.second;
        if (enemyPiece->is_white() == m_whiteToMove) {
            continue;
        }
        for (auto& move: enemyPiece->get_possible_moves()) {
            //kingside
            for (int i = 5; i < 8; i++) {
                if (move.endPosition == Position(i, y)) {
                    canKingside = false;
                }
            }
            //queenside
            for (int i = 3; i < 6; i++) {
                if (move.endPosition == Position(i, y)) {
                    canQueeside = false;
                }
            }
        }


    }
    if (canKingside) {
        castles.push_back(Move{Position(5, y),
                               Position(7, y),
                               EMPTY});

    }
    if (canQueeside) {
        castles.push_back(Move{Position(5, y),
                               Position(3, y),
                               EMPTY});

    }
    return castles;
}

std::vector<Move> Game::get_piece_possible_moves(Position position) {
    return m_piecesMap[position]->get_possible_moves();
}

std::vector<Position> Game::get_piece_moves_end_position(Position position) {
    std::vector<Move> possibleMoves = get_piece_possible_moves(position);
    std::vector<Position> endPositions;
    for(auto& move : possibleMoves){
        endPositions.push_back(move.endPosition);
    }
    return endPositions;
}









