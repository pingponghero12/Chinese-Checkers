#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

class Board {
private:
    int size;
    std::vector<std::vector<char>> Fileds;

    
    static constexpr char Alphabet[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

public:
    Board(int board_size);

    void fill_empty_space();
    void fill_top_triangle(char x);
    void fill_bottom_triangle(char x);
    void fill_top_left(char x);
    void fill_top_right(char x);
    void fill_bot_left(char x);
    void fill_bot_right(char x);
    void setup_board(int num_players);
    void move_checker(int x1, int y1, int x2, int y2);
    bool check_legality();
    void showBoard();
    void fill_hexagon();
    
};

#endif
