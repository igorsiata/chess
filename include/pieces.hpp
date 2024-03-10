#ifndef CHESS_PIECES_HPP
#define CHESS_PIECES_HPP

#include "types.hpp"
#include <cstdint>
#include <vector>
#include <map>


struct position{
    using Position = std::uint8_t;
    Position x;
    Position y;
};

class IPiece{
public:
    virtual position get_position() const=0;
    virtual bool is_white() const=0;
    virtual bool is_killed() const=0;
    virtual std::vector<position> get_possible_moves()const = 0;
    virtual void find_possible_moves(std::map<position, IPiece> const & allPieces) = 0;

};

class Bishop : IPiece{
public:
    Bishop(position position, bool white) : m_position(position), m_white(white), m_killed(false){};
    position get_position() const override{return m_position;}
    bool is_white() const override{return m_white;}
    bool is_killed() const override{return  m_killed;}
    std::vector<position> get_possible_moves()const override{return m_possibleMoves;}
    void find_possible_moves(std::map<position, IPiece> const & allPieces) override;
private:
    position m_position;
    bool m_white;
    bool m_killed;
    std::vector<position> m_possibleMoves;
};




#endif //CHESS_PIECES_HPP
