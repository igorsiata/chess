#ifndef CHESS_PIECES_HPP
#define CHESS_PIECES_HPP

#include "move.hpp"
#include <cstdint>
#include <utility>
#include <vector>
#include <map>


class Piece {
public:
    using PiecesMap = std::map<Position, Piece*>;

    Piece(Position position, bool white) : m_position(std::move(position)), m_white(white), m_id('q') {};

    Position get_position() const { return m_position; }

    bool is_white() const { return m_white; }

    std::vector<Move> get_possible_moves() const { return m_possibleMoves; }

    char get_id() const { return m_id; }

    void find_possible_moves(const PiecesMap& allPieces);

    virtual ~Piece() = default;

protected:
    virtual std::vector<Move> find_all_moves(const PiecesMap& allPieces) { return std::vector<Move>(); };

    bool is_check_after_move(const PiecesMap& allPieces, Move move);

    void set_possible_moves(std::vector<Move> possibleMoves) { m_possibleMoves = std::move(possibleMoves); };

    void set_id(char id) { m_id = id; };


    MoveType move_type(const std::map<Position, Piece*>& allPieces, Position move);

private:
    Position m_position;
    bool m_white;
    std::vector<Move> m_possibleMoves;
    char m_id;
};


class Bishop : public Piece {
public:
    Bishop(const Position& position, bool white);

    std::vector<Move> find_all_moves(const PiecesMap& allPieces) override;


};

class Pawn : public Piece {
public:
    Pawn(const Position& position, bool white);

    std::vector<Move> find_all_moves(const PiecesMap& allPieces) override;


};

class Rook : public Piece {
public:
    Rook(const Position& position, bool white);

    std::vector<Move> find_all_moves(const std::map<Position, Piece*>& allPieces) override;
};

class Kinght : public Piece {
public:
    Kinght(const Position& position, bool white);

    std::vector<Move> find_all_moves(const std::map<Position, Piece*>& allPieces) override;
};

class Queen : public Piece {
public:
    Queen(const Position& position, bool white);

    std::vector<Move> find_all_moves(const std::map<Position, Piece*>& allPieces) override;
};

class King : public Piece {
public:
    King(const Position& position, bool white);

    std::vector<Move> find_all_moves(const std::map<Position, Piece*>& allPieces) override;
};


#endif //CHESS_PIECES_HPP
