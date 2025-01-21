#ifndef fast_board_hpp
#define fast_board_hpp

#include <iostream>
#include <vector>
#include "board.hpp"

class Fast_Board : public Board {
    public:
        Fast_Board(int board_size);
        bool check_legality() override;
        std::vector<std::pair<int,int>> possible_shifts(int x, int y) override;
        std::vector<std::pair<int,int>>  possible_jumps(int x, int y, int a, int b) override;
        std::vector<std::pair<int, int>> possible_moves(int x, int y) override;
};

#endif // fast_board_hpp
