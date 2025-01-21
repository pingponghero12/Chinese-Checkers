#include "fast_board.hpp"

Fast_Board::Fast_Board(int board_size) : Board(board_size){
    //Constructor for the standard board
}

bool Fast_Board::check_legality() {
    //Check if the move is legal
    return true;
}

std::vector<std::pair<int, int>> Fast_Board::possible_moves(int x, int y){
    std::vector<std::pair<int, int>> result;
    std::vector<std::pair<int, int>> shifts = possible_shifts(x, y);
    std::vector<std::pair<int, int>> jumps = possible_jumps(x, y, x, y);
    result.insert(result.end(), shifts.begin(), shifts.end());
    result.insert(result.end(), jumps.begin(), jumps.end());
        
    return result;
}

std::vector<std::pair<int,int>> Fast_Board::possible_shifts(int x, int y){
    std::vector<std::pair<int, int>> shifts;
    if(get_Field(x + 1, y + 1)=='0'){
        shifts.push_back(std::make_pair(x+1, y+1));
    }
    if(get_Field(x - 1, y + 1)=='0'){
        shifts.push_back(std::make_pair(x-1, y+1));
    }
    if(get_Field(x + 1, y - 1)=='0'){
        shifts.push_back(std::make_pair(x+1, y-1));
    }
    if(get_Field(x - 1, y - 1)=='0'){
        shifts.push_back(std::make_pair(x-1, y-1));
    }
    if(get_Field(x, y + 2)=='0'){
        shifts.push_back(std::make_pair(x, y+2));
    }
    if(get_Field(x, y - 2)=='0'){
        shifts.push_back(std::make_pair(x, y-2));
    }
    return shifts;
}

//x and y are the point to calculate the jump from, a and b are the point from which previous jump was made
//This function has to guarrentee that you can't jump back to the same place
std::vector<std::pair<int,int>> Fast_Board::possible_jumps(int x, int y, int a, int b) {
    std::vector<std::pair<int, int>> jumps;

    auto add_jump = [&](int new_x, int new_y) {
        if (get_Field(new_x, new_y) == '0' && (new_x != a || new_y != b)) {
            jumps.push_back(std::make_pair(new_x, new_y));
            auto further_jumps = possible_jumps(new_x, new_y, x, y);
            jumps.insert(jumps.end(), further_jumps.begin(), further_jumps.end());
        }
    };


    //Fix the range
    for(int i = 1; i <=5; i++){
        if(get_Field(x, y + 2*i) != '0'){
            add_jump(x, y + 4*(i));
        }
        if(get_Field(x, y - 2*i) != '0'){
            add_jump(x, y - 4*(i));
        }   
        if(get_Field(x + i, y + i) != '0'){
            add_jump(x + 2*(i), y + 2*(i));
        }
        if(get_Field(x - i, y - i) != '0'){
            add_jump(x - 2*(i), y - 2*(i));
        }
        if(get_Field(x + i, y - i) != '0'){
            add_jump(x + 2*(i), y - 2*(i));
        }
        if(get_Field(x - i, y + i) != '0'){
            add_jump(x - 2*(i), y + 2*(i));
        }
    }

    return jumps;
}
