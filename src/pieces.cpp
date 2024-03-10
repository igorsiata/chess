#include "pieces.hpp"

void Bishop::find_possible_moves(std::map<position, IPiece> const & allPieces) {
    std::vector<position> possibleMoves;
    while(sth){
        for(int i = -1;i<=1;i+=2){
            for(int j = -1;j<=1;j+=2){
                position possibleMove = position{
                        static_cast<position::Position>(m_position.x + i),
                        static_cast<position::Position>(m_position.y + j)
                };
                if(allPieces.count(possibleMove) == 0){

                }
            }
        }

    }
}
