#ifndef CHESS_PIECES_HPP
#define CHESS_PIECES_HPP

#include <cstdint>
#include <vector>
#include <map>


struct position{
    using Position = std::uint8_t;
    position(Position x, Position y) : x(x), y(y){}
    Position x;
    Position y;
    bool operator<(const position& other) const {
        return (x < other.x) || ((x == other.x) && (y < other.y));
    }
};

class Piece{
public:
    Piece(const position& position, bool white) : m_position(position), m_white(white), m_killed(false), m_id('q'){};
    position get_position() const {return m_position;}
    bool is_white() const {return m_white;}
    bool is_killed() const {return  m_killed;}
    std::vector<position> get_possible_moves()const {return m_possibleMoves;}
    char get_id() const{return m_id;}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    virtual void find_possible_moves(const std::map<position, Piece*>& allPieces) { return; };
#pragma clang diagnostic pop
    virtual ~Piece() = default;

protected:
    position m_position;
    bool m_white;
    bool m_killed;
    std::vector<position> m_possibleMoves;
    char m_id;
};


class Bishop : public Piece {
public:
    Bishop(const position& position, bool white);
    void find_possible_moves(const std::map<position, Piece*>& allPieces)override;

};

class Pawn : public Piece{
public:
    Pawn(const position& position, bool white);
    void find_possible_moves(const std::map<position, Piece*>& allPieces)override;
};

class Rook : public Piece{
public:
    Rook(const position& position, bool white);
    void find_possible_moves(const std::map<position, Piece*>& allPieces)override;
};

class Kinght : public Piece{
public:
    Kinght(const position& position, bool white);
    void find_possible_moves(const std::map<position, Piece*>& allPieces)override;
};

class Queen : public Piece{
public:
    Queen(const position& position, bool white);
    void find_possible_moves(const std::map<position, Piece*>& allPieces)override;
};

class King : public Piece{
public:
    King(const position& position, bool white);
    void find_possible_moves(const std::map<position, Piece*>& allPieces)override;
};




#endif //CHESS_PIECES_HPP
