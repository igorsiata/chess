#pragma once

#include "game_logic/board.hpp"
#include <map>
#include <string>

namespace FENParser {
void loadPosition(const std::string &positionFEN, Board *board);
void loadPieces(const std::string &positionFEN, Board *board);
void loadPlayerToMove(const std::string &positionFEN, Board *board);
void loadAvailableCastles(const std::string &positionFEN, Board *board);
void loadEnPassantSquare(const std::string &positionFEN, Board *board);
void loadCounters(const std::string &positionFEN, Board *board);
} // namespace FENParser