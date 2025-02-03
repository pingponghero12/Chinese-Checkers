#ifndef Bot_hpp
#define Bot_hpp

#include <vector>
#include <memory>

#include "board.hpp"

class Bot  {
    private:
        std::shared_ptr<Board> board;
        char color;
        std::vector<std::vector<int>> checkers;
        std::vector<std::pair<int, int>> destination;
        int x1, y1, x2, y2;

    public:
        Bot(std::shared_ptr<Board> board);
        void set_color(int color_int);
        void get_checkers();
        void set_destination();
        int get_distance(int x1, int y1);
        void make_move();
        int get_x1();
        int get_y1();
        int get_x2();
        int get_y2();
        
};

#endif // Bot_hpp
