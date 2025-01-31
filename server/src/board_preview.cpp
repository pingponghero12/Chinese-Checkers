#include <iostream>
#include "standard_board.hpp"

/**
 * Quick funtion to test the board
 * Initialize the board with a size of 5
 * Setup the board with pieces for 6 players
 * Show the board
 */
int main_main(){
    Standard_Board board(5);

    board.setup_board(6);
    board.showBoard();
    
    int x1, y1, x2, y2;
    while(true){
        std::cin >> y1;
        std::cin >> x1;
        auto moves = board.possible_moves(y1, x1);
        for(auto move : moves){
            std::cout << move.first << " " << move.second << std::endl;
        }
        std::cin >> y2;
        std::cin >> x2;
        board.move(y1, x1, y2, x2);
        board.showBoard();
    }
}
