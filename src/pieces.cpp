#include "pieces.hpp"

Bishop::Bishop(const position& position, bool white) : Piece(position, white) {
    if(white)
        m_id = 'B';
    else
        m_id = 'b';
}

void Bishop::find_possible_moves(const std::map<position, Piece*> & allPieces) {
    std::vector<position> possibleMoves;

        for(int i = -1;i<=1;i+=2){
            for(int j = -1;j<=1;j+=2){
                for(std::size_t step = 1;step<=8;step++){
                    position possibleMove = position{
                        static_cast<position::Position>(m_position.x + i*step),
                        static_cast<position::Position>(m_position.y + j*step)
                    };
                    if(possibleMove.x<1 || possibleMove.x>8 ||possibleMove.y<1 || possibleMove.y>8){
                        break;
                    }
                    auto it = allPieces.find(possibleMove);
                    if(it != allPieces.end()){
                        if (it->second->is_white() != m_white){
                            possibleMoves.push_back(possibleMove);
                        }
                        break;
                    }
                    else
                        possibleMoves.push_back(possibleMove);
                }
            }
        }

}


Pawn::Pawn(const position& position, bool white) : Piece(position, white) {
    if(white)
        m_id = 'P';
    else
        m_id = 'p';
}

void Pawn::find_possible_moves(const std::map<position, Piece*>& allPieces) {
    Piece::find_possible_moves(allPieces);
}

Rook::Rook(const position& position, bool white) : Piece(position, white) {
    if(white)
        m_id = 'R';
    else
        m_id = 'r';
}

void Rook::find_possible_moves(const std::map<position, Piece*>& allPieces) {
    Piece::find_possible_moves(allPieces);
}

Kinght::Kinght(const position& position, bool white) : Piece(position, white) {
    if(white)
        m_id = 'N';
    else
        m_id = 'n';
}

void Kinght::find_possible_moves(const std::map<position, Piece*>& allPieces) {
    Piece::find_possible_moves(allPieces);
}

Queen::Queen(const position& position, bool white) : Piece(position, white) {
    if(white)
        m_id = 'Q';
    else
        m_id = 'q';
}

void Queen::find_possible_moves(const std::map<position, Piece*>& allPieces) {
    Piece::find_possible_moves(allPieces);
}

King::King(const position& position, bool white) : Piece(position, white) {
    if(white)
        m_id = 'K';
    else
        m_id = 'k';
}

void King::find_possible_moves(const std::map<position, Piece*>& allPieces) {
    Piece::find_possible_moves(allPieces);
}