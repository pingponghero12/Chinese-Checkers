#ifndef standard_board_hpp
#define standard_board_hpp

#include <iostream>
#include <vector>
#include "board.hpp"

class Standard_Board : public Board {
    public:
        Standard_Board(int board_size);
        bool check_legality() override;
        void possible_shifts(int x, int y);
        void possible_jumps(int x, int y);
        void next_jump(int x, int y, int a, int b);
};

#endif // standard_board_hpp