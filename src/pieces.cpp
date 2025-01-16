#include "pieces.hpp"

MoveType Piece::move_type(const PiecesMap& allPieces, Position move) const {
    //is move out of board
    if (move.first < 1 || move.first > 8 || move.second < 1 || move.second > 8)
        return MoveType::OUT;

    //is move colliding with any other piece
    auto itr = allPieces.find(move);
    if (itr == allPieces.end()) {
        return MoveType::EMPTY;
    }
    if (itr->second->is_white() == m_white) {
        return MoveType::ALLY;
    } else {
        if (itr->second->get_id() == 'K' || itr->second->get_id() == 'k') {
            return MoveType::CHECK;
        } else {
            return MoveType::CAPTURE;
        }

    }


}


bool Piece::is_check_after_move(const PiecesMap& allPieces,
                                const Position& piecePosition,
                                const Move& move) const {
    //create dummy pieces map after move
    PiecesMap dummyPiecesMap = allPieces;
    if (move.moveType == CAPTURE) {
        auto capturedPiece = dummyPiecesMap.find(move.endPosition);
        dummyPiecesMap.erase(capturedPiece);
    }
    if (move.moveType == ENPASSAT) {
        if (m_white) {
            dummyPiecesMap.erase(Position(move.endPosition.first, 4));
        } else {
            dummyPiecesMap.erase(Position(move.endPosition.first, 5));
        }
    }
    auto itr = dummyPiecesMap.find(piecePosition);
    if (itr != dummyPiecesMap.end()) {
        std::swap(dummyPiecesMap[move.endPosition], itr->second);
        dummyPiecesMap.erase(itr);
    }


    //check for checks
    for (auto elem: dummyPiecesMap) {
        Piece* piece_ptr = elem.second;
        if (piece_ptr->is_white() != m_white) {
            std::vector<Move> possibleMoves = piece_ptr->find_all_moves(dummyPiecesMap);
            for (auto enemyMove: possibleMoves) {
                if (enemyMove.moveType == CHECK) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Piece::find_possible_moves(const Piece::PiecesMap& allPieces, const bool isWhiteMove) {
    if (isWhiteMove != m_white) {
        set_possible_moves(std::vector<Move>());
        return false;
    }
    std::vector<Move> allMoves = find_all_moves(allPieces);
    allMoves.erase(std::remove_if(
                           allMoves.begin(), allMoves.end(),
                           [this, allPieces](auto itr) {
                               return is_check_after_move(allPieces, m_position, itr);
                           }),
                   allMoves.end());

    set_possible_moves(allMoves);

    return !allMoves.empty();

}

void Piece::add_possible_moves(const std::vector<Move>& extraMoves) {
    for (auto move: extraMoves) {
        m_possibleMoves.push_back(move);
    }
}

void Piece::add_possible_moves(const Move& extraMove) {
    m_possibleMoves.push_back(extraMove);
}


Bishop::Bishop(const Position& position, bool white) : Piece(position, white) {
    if (white)
        set_id('B');
    else
        set_id('b');
}

std::vector<Move> Bishop::find_all_moves(const PiecesMap& allPieces) {
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int step = 1; step <= 8; step++) {
                endPosition = Position(
                        get_position().first + i * step,
                        get_position().second + j * step);
                moveType = move_type(allPieces, endPosition);
                move = Move{endPosition, moveType};
                switch (moveType) {
                    case OUT:
                        goto exit_loop;
                    case EMPTY:
                        possibleMoves.push_back(move);
                        break;
                    case CAPTURE:
                        possibleMoves.push_back(move);
                        goto exit_loop;
                    case ALLY:
                        goto exit_loop;
                    case CHECK:
                        possibleMoves.push_back(move);
                        goto exit_loop;
                }
            }
            exit_loop:;
        }
    }
    return possibleMoves;
}


Pawn::Pawn(const Position& position, bool white) : Piece(position, white) {
    if (white)
        set_id('P');
    else
        set_id('p');
}

std::vector<Move> Pawn::find_all_moves(const std::map<Position, Piece*>& allPieces) {
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int color_mult = 1;
    if (is_white()) {
        color_mult = -1;
    }

    //move 2 squares
    if ((is_white() && get_position().second == 7)
        || (!is_white() && get_position().second == 2)) {
        endPosition = Position(
                get_position().first,
                get_position().second + 2 * color_mult);
        moveType = move_type(allPieces, endPosition);
        move = Move{endPosition, moveType};
        if (move_type(allPieces, endPosition) == EMPTY) {
            possibleMoves.push_back(move);
        }
    }

    //move 1 square
    endPosition = Position(
            get_position().first,
            get_position().second + 1 * color_mult);
    moveType = move_type(allPieces, endPosition);
    move = Move{endPosition, moveType};
    if (move_type(allPieces, endPosition) == EMPTY) {
        possibleMoves.push_back(move);
    } else if (!possibleMoves.empty()) {
        possibleMoves.pop_back();
    }

    //captures
    for (int i = -1; i <= 1; i += 2) {
        endPosition = Position(
                get_position().first + i,
                get_position().second + 1 * color_mult);
        moveType = move_type(allPieces, endPosition);
        move = Move{endPosition, moveType};
        if (move_type(allPieces, endPosition) == CAPTURE
            || move_type(allPieces, endPosition) == CHECK) {
            possibleMoves.push_back(move);
        }
    }
    return possibleMoves;
}


Rook::Rook(const Position& position, bool white) : Piece(position, white) {
    if (white)
        set_id('R');
    else
        set_id('r');
}

std::vector<Move> Rook::find_all_moves(const std::map<Position, Piece*>& allPieces) {
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    for (int i = 0; i < 4; i++) {
        for (int step = 1; step <= 8; step++) {
            endPosition = Position(
                    get_position().first + dx[i] * step,
                    get_position().second + dy[i] * step);
            moveType = move_type(allPieces, endPosition);
            move = Move{endPosition, moveType};
            switch (moveType) {
                case OUT:
                    goto exit_loop;
                case EMPTY:
                    possibleMoves.push_back(move);
                    break;
                case CAPTURE:
                    possibleMoves.push_back(move);
                    goto exit_loop;
                case ALLY:
                    goto exit_loop;
                case CHECK:
                    possibleMoves.push_back(move);
                    goto exit_loop;
            }
        }
        exit_loop:;
    }

    return possibleMoves;

}


Kinght::Kinght(const Position& position, bool white) : Piece(position, white) {
    if (white)
        set_id('N');
    else
        set_id('n');
}

std::vector<Move> Kinght::find_all_moves(const std::map<Position, Piece*>& allPieces) {
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int dx[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    int dy[8] = {1, -1, 2, -2, 2, -2, 1, -1};
    for (int i = 0; i < 8; i++) {
        endPosition = Position(
                get_position().first + dx[i],
                get_position().second + dy[i]);
        moveType = move_type(allPieces, endPosition);
        move = Move{endPosition, moveType};
        switch (moveType) {
            case OUT:
                break;
            case EMPTY:
                possibleMoves.push_back(move);
                break;
            case CAPTURE:
                possibleMoves.push_back(move);
                break;
            case ALLY:
                break;
            case CHECK:
                possibleMoves.push_back(move);
                break;
        }
    }
    return possibleMoves;

}


Queen::Queen(const Position& position, bool white) : Piece(position, white) {
    if (white)
        set_id('Q');
    else
        set_id('q');
}

std::vector<Move> Queen::find_all_moves(const std::map<Position, Piece*>& allPieces) {
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int dx[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    int dy[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
    for (int i = 0; i < 9; i++) {
        for (int step = 1; step <= 8; step++) {
            endPosition = Position(
                    get_position().first + dx[i] * step,
                    get_position().second + dy[i] * step);
            moveType = move_type(allPieces, endPosition);
            move = Move{endPosition, moveType};
            switch (moveType) {
                case OUT:
                    goto exit_loop;
                case EMPTY:
                    possibleMoves.push_back(move);
                    break;
                case CAPTURE:
                    possibleMoves.push_back(move);
                    goto exit_loop;
                case ALLY:
                    goto exit_loop;
                case CHECK:
                    possibleMoves.push_back(move);
                    goto exit_loop;
            }
        }
        exit_loop:;
    }

    return possibleMoves;
}


King::King(const Position& position, bool white) : Piece(position, white) {
    if (white)
        set_id('K');
    else
        set_id('k');
}

std::vector<Move> King::find_all_moves(const std::map<Position, Piece*>& allPieces) {
    std::vector<Move> possibleMoves;
    MoveType moveType;
    Position endPosition;
    Move move;
    int dx[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    int dy[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
    for (int i = 0; i < 9; i++) {
        endPosition = Position(
                get_position().first + dx[i],
                get_position().second + dy[i]);
        moveType = move_type(allPieces, endPosition);
        move = Move{endPosition, moveType};
        switch (moveType) {
            case OUT:
                break;
            case EMPTY:
                possibleMoves.push_back(move);
                break;
            case CAPTURE:
                possibleMoves.push_back(move);
                break;
            case ALLY:
                break;
            case CHECK:
                possibleMoves.push_back(move);
                break;
        }
    }
    return possibleMoves;
}












