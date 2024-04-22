#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP

#include "pieces.hpp"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


class Game {
public:
    Game(const std::string& positionAsString
    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");


    bool move(const Position& startPosition, const Position& endPosition);


    std::vector<Move> get_piece_possible_moves(Position position);

    std::map<Position, char> get_all_pieces();

private:
    bool generate_possible_moves();

    void rook_move(Position startPosition);

    void en_passnat_square(Piece* pieceToMove, Position startPosition, Position endPosition);

    void castle(Move moveItr);

    void find_enpassant();

    void load_position(const std::string& positionAsString);

    std::vector<Move> find_castles();

    Piece::PiecesMap m_piecesMap;
    bool m_whiteToMove;
    Position m_enPassantSquare;
    std::map<char, bool> m_CastlesAvailable;

};


#endif //CHESS_GAME_HPP
