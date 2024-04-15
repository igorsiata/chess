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
    Game();

    void move(const Move& move);

    void load_position(const std::string& positionAsString);

    void generate_position(std::ostream& os);

    void generate_possible_moves();

    void output_possible_moves(std::ostream& os);

    std::vector<Move> get_piece_possible_moves(Position position);

    std::vector<Position> get_piece_moves_end_position(Position position);

private:
    std::vector<Move> find_castles();
    Piece::PiecesMap m_piecesMap;
    bool m_whiteToMove;
    Position m_enPassantSquare;
    std::map<char, bool> m_CastlesAvailable;

};


#endif //CHESS_GAME_HPP
