#include <iostream>
#include "standard_board.hpp"

int chuj(){

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
