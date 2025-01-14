#ifndef standard_board_hpp
#define standard_board_hpp

#include <iostream>
#include <vector>
#include "board.hpp"

class Standard_Board : public Board {
    public:
        Standard_Board(int board_size);
        bool check_legality() override;
};

#endif // standard_board_hpp