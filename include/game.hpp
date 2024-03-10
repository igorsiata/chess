#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP
#include "chessboard.hpp"
class Game{
public:
    using PiecesMap = std::map<const position, IPiece*>;
    void move(position startPosition, position endPosition);
private:
    PiecesMap m_piecesMap;
    std::vector<IPiece> m_allPieces;
};


#endif //CHESS_GAME_HPP
