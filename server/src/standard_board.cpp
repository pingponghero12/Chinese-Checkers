#include "standard_board.hpp"

Standard_Board::Standard_Board(int board_size) : Board(board_size){
    //Constructor for the standard board
}

bool Standard_Board::check_legality() {
    //Check if the move is legal
    return true;
}



void Standard_Board::possible_shifts(int x, int y){
    //Check if the shift is possible
    if(get_Field(x + 1, y + 1)=='0'){
        std::cout << "Possible move to " << x+1 << " " << y+1 << std::endl;
    }
    if(get_Field(x - 1, y + 1)=='0'){
        std::cout << "Possible move to " << x-1 << " " << y+1 << std::endl;
    }
    if(get_Field(x + 1, y - 1)=='0'){
        std::cout << "Possible move to " << x+1 << " " << y-1 << std::endl;
    }
    if(get_Field(x - 1, y - 1)=='0'){
        std::cout << "Possible move to " << x-1 << " " << y-1 << std::endl;
    }
    if(get_Field(x, y + 2)=='0'){
        std::cout << "Possible move to " << x << " " << y+2 << std::endl;
    }
    if(get_Field(x, y - 2)=='0'){
        std::cout << "Possible move to " << x << " " << y-2 << std::endl;
    }
}

void Standard_Board::possible_jumps(int x, int y){
    //Check if the jump is possible
    if(get_Field(x-1, y+1) != '0' && get_Field(x-2, y+2)=='0'){
        std::cout << "Possible jump to " << x-2 << " " << y+2 << std::endl;
        next_jump(x-2, y+2, x, y);
    }
    if(get_Field(x+1, y-1)!='0' && get_Field(x+2, y-2)=='0'){
        std::cout << "Possible jump to " << x+2 << " " << y-2 << std::endl;
        next_jump(x+2, y-2, x, y);
    }
    if(get_Field(x-1, y-1) != '0' && get_Field(x-2, y-2) == '0'){
        std::cout << "Possible jump to " << x-2 << " " << y-2 << std::endl;
        next_jump(x-2, y-2, x, y);
    }
    if(get_Field(x+1, y+1) != '0' && get_Field(x+2, y+2) == '0'){
        std::cout << "Possible jump to " << x+2 << " " << y+2 << std::endl;
        next_jump(x+2, y+2, x, y);
    }
}

//x and y are the point to calculate the jump from, a and b are the point from which previous jump was made
//This function has to guarrentee that you can't jump back to the same place
void Standard_Board::next_jump(int x, int y, int a , int b){
    if(get_Field(x-1, y-1) != '0' && get_Field(x-2, y-2) == '0' && (x-2 != a || y-2 != b)){
        std::cout << "Possible jump to " << x-2 << " " << y-2 << std::endl;
        next_jump(x-2, y-2, x, y);
    }
    if(get_Field(x+1, y+1) != '0' && get_Field(x+2, y+2) == '0' && (x+2 != a || y+2 != b)){
        std::cout << "Possible jump to " << x+2 << " " << y+2 << std::endl;
        next_jump(x+2, y+2, x, y);
    }
    if(get_Field(x-1, y+1) != '0' && get_Field(x-2, y+2) == '0' && (x-2 != a || y+2 != b)){
        std::cout << "Possible jump to " << x-2 << " " << y+2 << std::endl;
        next_jump(x-2, y+2, x, y);
    }
    if(get_Field(x+1, y-1) != '0' && get_Field(x+2, y-2) == '0' && (x+2 != a || y-2 != b)){
        std::cout << "Possible jump to " << x+2 << " " << y-2 << std::endl;
        next_jump(x+2, y-2, x, y);
    }
}