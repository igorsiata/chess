#include "game.hpp"


Game::Game(const std::string& positionAsString) {
    load_position(positionAsString);
    generate_possible_moves();
}

bool Game::move(const Position& startPosition, const Position& endPosition) {
    if (m_piecesMap.find(startPosition) == m_piecesMap.end()) {
        return true;
    }
    Piece* pieceToMove = m_piecesMap[startPosition];
    auto possibleMoves = pieceToMove->get_possible_moves();
    Move thisMove;
    for (auto moveItr: possibleMoves) {

        if (moveItr.endPosition == endPosition) {
            thisMove = moveItr;
            if (thisMove.moveType == CASTLE) {
                castle(moveItr);
            }
            if (thisMove.moveType == CAPTURE) {
                m_piecesMap.erase(thisMove.endPosition);
            }
            if (thisMove.moveType == ENPASSAT) {
                if (m_whiteToMove) {
                    m_piecesMap.erase(Position(m_enPassantSquare.first, 4));
                } else {
                    m_piecesMap.erase(Position(m_enPassantSquare.first, 5));
                }
            }
            //black king move
            if (startPosition.first == 5 and startPosition.second == 1) {
                m_CastlesAvailable['k'] = false;
                m_CastlesAvailable['q'] = false;
            }
            // white king move
            if (startPosition.first == 5 and startPosition.second == 8) {
                m_CastlesAvailable['K'] = false;
                m_CastlesAvailable['Q'] = false;
            }
            en_passnat_square(pieceToMove, startPosition, endPosition);
            rook_move(startPosition);
            rook_move(endPosition);


            auto itr = m_piecesMap.find(startPosition);
            itr->second->change_position(endPosition);
            if (itr != m_piecesMap.end()) {
                // Swap value from oldKey to newKey, note that a default constructed value
                // is created by operator[] if 'm' does not contain newKey.
                std::swap(m_piecesMap[thisMove.endPosition], itr->second);
                // Erase old key-value from map
                m_piecesMap.erase(itr);
            }
            m_whiteToMove = !m_whiteToMove;
            return generate_possible_moves();
        }

    }

    return true;
}

void Game::load_position(const std::string& positionAsString) {
    m_CastlesAvailable = {{'K', false},
                          {'Q', false},
                          {'k', false},
                          {'q', false}};
    m_enPassantSquare = Position(0, 0);
    Position currentPosition(1, 1);
    int loadState = 0;
    for (auto it: positionAsString) {
        if (it == ' ') {
            loadState++;
        } else if (loadState == 0) {
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

        } else if (loadState == 1) {
            if (it == 'w') {
                m_whiteToMove = true;
            } else if (it == 'b') {
                m_whiteToMove = false;
            }

        } else if (loadState == 2) {
            m_CastlesAvailable[it] = true;
        } else if (loadState == 3) {
            if (it == '-') {
                loadState++;
            } else if (m_enPassantSquare.first != 0) {
                m_enPassantSquare.second = (int) it;
            } else {
                m_enPassantSquare.first = ((int) it) - 96;
            }


        } else {
            break;
        }
    }

}


bool Game::generate_possible_moves() {
    bool isCheckmate = true;
    for (auto& it: m_piecesMap) {
        if (it.second->find_possible_moves(m_piecesMap, m_whiteToMove)) {
            isCheckmate = false;
        }
    }
    //add castles
    std::vector<Move> castles = find_castles();
    if (!castles.empty()) {
        int y = 1 + int(m_whiteToMove) * 7;
        auto kingItr = m_piecesMap.find(Position(5, y));
        kingItr->second->add_possible_moves(castles);
    }
    find_enpassant();
    if (isCheckmate) {
        return false;
    }
    return true;
    //add en passant square
}


std::vector<Move> Game::find_castles() {
    std::vector<Move> castles;
    bool canKingside;
    bool canQueeside;
    if (m_whiteToMove) {
        canKingside = m_CastlesAvailable['K'];
        canQueeside = m_CastlesAvailable['Q'];
    } else {
        canKingside = m_CastlesAvailable['k'];
        canQueeside = m_CastlesAvailable['q'];
    }
    if (!canKingside and !canQueeside) {
        return castles;
    }
    // y -> row where king stands
    int y = 1 + int(m_whiteToMove) * 7;

    auto kingItr = m_piecesMap.find(Position(5, y));


    //squares to check if they are empty and not under attack

    //empty kingside
    for (int i = 6; i < 8; i++) {
        if (m_piecesMap.find(Position(i, y)) != m_piecesMap.end()) {
            canKingside = false;
        }
    }

    //empty queenside
    for (int i = 2; i < 5; i++) {
        if (m_piecesMap.find(Position(i, y)) != m_piecesMap.end()) {
            canQueeside = false;
        }
    }

    //check if squares are under attack
    for (auto& elem: m_piecesMap) {
        Piece* enemyPiece = elem.second;
        if (enemyPiece->is_white() == m_whiteToMove) {
            continue;
        }
        for (auto& move: enemyPiece->find_all_moves(m_piecesMap)) {
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
        castles.push_back(Move{Position(7, y),
                               CASTLE});

    }
    if (canQueeside) {
        castles.push_back(Move{Position(3, y),
                               CASTLE});

    }
    return castles;
}

std::vector<Move> Game::get_piece_possible_moves(Position position) {
    if (m_piecesMap.find(position) == m_piecesMap.end()) {
        return std::vector<Move>();
    }
    return m_piecesMap[position]->get_possible_moves();
}


std::map<Position, char> Game::get_all_pieces() {
    std::map<Position, char> allPieces;
    for (auto elem: m_piecesMap) {
        allPieces[elem.first] = elem.second->get_id();
    }
    return allPieces;
}


void Game::castle(Move moveItr) {
    int y = moveItr.endPosition.second;
    if (moveItr.endPosition.first == 7) {
        move(Position(8, y), Position(6, y));
        m_whiteToMove = !m_whiteToMove;
    }
    if (moveItr.endPosition.first == 3) {
        move(Position(1, y), Position(4, y));
        m_whiteToMove = !m_whiteToMove;
    }
    if (m_whiteToMove) {
        m_CastlesAvailable['K'] = false;
        m_CastlesAvailable['Q'] = false;
    } else {
        m_CastlesAvailable['k'] = false;
        m_CastlesAvailable['q'] = false;
    }

}

void Game::en_passnat_square(Piece* pieceToMove, Position startPosition, Position endPosition) {
    if (pieceToMove->get_id() == 'p' or pieceToMove->get_id() == 'P') {
        if (startPosition.second - endPosition.second == 2) {
            m_enPassantSquare = Position(startPosition.first, 6);
            return;
        }
        if (startPosition.second - endPosition.second == -2) {
            m_enPassantSquare = Position(startPosition.first, 3);
            return;
        }
    }
    m_enPassantSquare = Position(0, 0);
}

void Game::rook_move(Position startPosition) {
    if (startPosition.first == 1 && startPosition.second == 1) {
        m_CastlesAvailable['q'] = false;
    }
    if (startPosition.first == 8 && startPosition.second == 1) {
        m_CastlesAvailable['k'] = false;
    }
    if (startPosition.first == 1 && startPosition.second == 8) {
        m_CastlesAvailable['Q'] = false;
    }
    if (startPosition.first == 8 && startPosition.second == 8) {
        m_CastlesAvailable['K'] = false;
    }
}

void Game::find_enpassant() {
    Position pos1(m_enPassantSquare.first - 1, 5);
    Position pos2(m_enPassantSquare.first + 1, 5);
    if (m_whiteToMove) {
        pos1.second = 4;
        pos2.second = 4;
    }
    auto itr1 = m_piecesMap.find(pos1);
    auto itr2 = m_piecesMap.find(pos2);
    if (itr1 != m_piecesMap.end()) {
        if (itr1->second->get_id() == 'P' or itr1->second->get_id() == 'p') {
            Move move = {m_enPassantSquare, ENPASSAT};
            if (!itr1->second->is_check_after_move(m_piecesMap, pos1, move))
                itr1->second->add_possible_moves(move);
        }
    }
    if (itr2 != m_piecesMap.end()) {
        if (itr2->second->get_id() == 'P' or itr2->second->get_id() == 'p') {
            Move move = {m_enPassantSquare, ENPASSAT};
            if (!itr2->second->is_check_after_move(m_piecesMap, pos2, move))
                itr2->second->add_possible_moves(move);
        }
    }
}









