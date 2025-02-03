#ifndef standard_board_hpp
#define standard_board_hpp

#include <vector>
#include "board.hpp"

/**
 * @brief Standard_Board class
 * The class represents the standard board for the game
 * The class is initialized with a size
 * The class is responsible for checking the legality of moves
 */
class Standard_Board : public Board {
    public:
        Standard_Board(int board_size);
        bool check_legality() override;
        std::vector<std::pair<int,int>> possible_shifts(int x, int y) override;
        std::vector<std::pair<int,int>>  possible_jumps(int x, int y, int a, int b) override;
        std::vector<std::pair<int, int>> possible_moves(int x, int y);
};

#endif // standard_board_hpp
