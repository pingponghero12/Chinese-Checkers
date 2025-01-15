#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

class Board {
private:
    int size;
    std::vector<std::vector<char>> Fileds;

public:
    Board(int board_size);
    virtual ~Board() = default;

    void fill_empty_space();
    void fill_top_triangle(char x);
    void fill_bottom_triangle(char x);
    void fill_top_left(char x);
    void fill_top_right(char x);
    void fill_bot_left(char x);
    void fill_bot_right(char x);
    void setup_board(int num_players);
    void move(int x1, int y1, int x2, int y2);
    virtual bool check_legality() = 0;
    void showBoard();
    void fill_hexagon();
    virtual std::vector<std::pair<int,int>> possible_shifts(int x, int y) = 0;
    virtual std::vector<std::pair<int,int>> possible_jumps(int x, int y, int a, int b) = 0;
    virtual std::vector<std::pair<int, int>> possible_moves(int x, int y) = 0;
    char get_Field(int x, int y) const;
    std::vector<std::vector<char>> getFields() const;
    void set_Field(int x, int y, char value);
    int get_size() const;

    
};

#endif
