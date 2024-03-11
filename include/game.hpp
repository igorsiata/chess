#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP

#include "pieces.hpp"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


class Game{
public:
    using PiecesMap = std::map<position, Piece*>;
    void move(const position& startPosition, const position& endPosition);
    void load_position(const std::string& positionAsString);
    void generate_position(std::ostream& os);
    void generate_possible_moves();
private:
    PiecesMap m_piecesMap;
    std::vector<Piece> m_allPieces;
};



#endif //CHESS_GAME_HPP
