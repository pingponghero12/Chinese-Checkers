#include "standard_board.hpp"

Standard_Board::Standard_Board(int board_size) : Board(board_size){
    //Constructor for the standard board
}

bool Standard_Board::check_legality() {
    //Check if the move is legal
    return true;
}
