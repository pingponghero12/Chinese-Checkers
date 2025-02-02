#ifndef Bot_hpp
#define Bot_hpp

#include <iostream>
#include <vector>
#include "board.hpp"

class Bot  {
    private:
        Board* board;
        char color;
        std::vector<std::vector<int>> checkers;
        std::vector<std::pair<int, int>> destination;

    public:
        Bot(Board* board);
        void set_color(char color);
        void get_checkers();
        void set_destination();
        int get_distance(int x1, int y1);
        void make_move();
        
};

#endif // Bot_hpp