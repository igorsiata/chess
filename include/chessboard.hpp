#ifndef CHESS_CHESSBOARD_HPP
#define CHESS_CHESSBOARD_HPP

#include "pieces.hpp"
#include <map>
#include <string>

class BoardState{
public:
    std::string get_position_as_string()  const {return m_positionAsString;};
    void set_position(std::string positionAsString);
    void start_game();
private:
    std::map<position, IPiece> m_positions;
    std::string m_positionAsString;
};


#endif //CHESS_CHESSBOARD_HPP
