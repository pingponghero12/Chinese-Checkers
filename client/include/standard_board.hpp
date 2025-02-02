#ifndef standard_board_hpp
#define standard_board_hpp

#include <vector>
#include "board.hpp"

class Standard_Board : public Board {
    public:
        Standard_Board(int board_size);
        bool check_legality() override;
        std::vector<std::pair<int,int>> possible_shifts(int x, int y) override;
        std::vector<std::pair<int,int>>  possible_jumps(int x, int y, int a, int b) override;
        std::vector<std::pair<int, int>> possible_moves(int x, int y) override;
};

#endif // standard_board_hpp
