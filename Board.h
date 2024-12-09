#ifndef Board_H
#define Board_H

#include <iostream>

class Board{
    private:
    int size;
    int first_row_pins;
    int middle = WIDTH/2;
    int left_middle=first_row_pins-1;
    int right_middle = WIDTH - left_middle;

    int HEIGHT = 2*size - 1 + 2*first_row_pins;
    int WIDTH = ((size + 2*first_row_pins)-1)*2;

    //TODO: Fix error - change to vector
    char Fileds[HEIGHT][WIDTH+1]; 
    

    //Delete in future
    char Alphabet[27]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    public:
    //Constructor
    Board(int board_size);


    void fill_top_triangle(char x);
    void fill_bottom_trinalge(char x);
    void fill_top_left(char x);
    void fill_top_right(char x);
    void fill_bot_left(char x);
    void fill_bot_right(char x);
    void setup_board(int num_players);
    void move_checker();
    void check_legality();
    void showBoard();

};


#endif 