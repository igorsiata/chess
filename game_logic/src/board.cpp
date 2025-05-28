#include "board.hpp"

int Array120To64(int position){
    int numberOfRows = position/10;
    return position - 20 - 2*numberOfRows;
}

int Array64To120(int position){
    int numberOfRows = position/8;
    return position + 20 + 2*numberOfRows;
}