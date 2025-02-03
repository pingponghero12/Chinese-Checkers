#include <vector>
#include <cmath>

#include "board.hpp"
#include "bot.hpp"

//Możliwe że będzie potrzebny osobny bot do każdej planszy

Bot::Bot(std::shared_ptr<Board> board) : board(board) {
    //Constructor for the bot
}

void Bot::set_color(int color_int) {
    char color_char = '0' + color_int;
    this->color = color_char;
}

void Bot::get_checkers() {
    for (int i = 0; i <= 4 * (board->get_size() - 1); i++) {
        for (int j = 0; j <= 6 * (board->get_size() - 1); j++) {
            if (board->get_Field(i, j) == color) {
                checkers.push_back(std::vector<int>{i, j});
            }
        }
    }
}
//add to constructor?
void Bot::set_destination(){
    for(auto checker : checkers){
        if(checker[0] == 0 && checker [1] == 12){
            destination.push_back(std::make_pair(16, 12));
        }

        if(checker[0] == 16 && checker [1] == 12){
            destination.push_back(std::make_pair(0, 12));
            
        }

        if(checker[0] == 4 && checker [1] == 0){
            destination.push_back(std::make_pair(12, 24));
        }

        if(checker[0] == 12 && checker [1] == 24){
            destination.push_back(std::make_pair(4, 0));
        }

        if(checker[0] == 4 && checker [1] == 24){
            destination.push_back(std::make_pair(12, 0));
        }

        if(checker[0] == 12 && checker [1] == 0){
            destination.push_back(std::make_pair(4, 24));
        }
    }
}

//possible improvemnt
int Bot::get_distance(int x1, int y1){
    return pow(abs(x1 - destination[0].first), 2) + pow(abs(y1 - destination[0].second), 2);
}

void Bot::make_move(){
    int current_distance = 0;
    int difference = 0;
    std::vector<int> best_checker;
    std::pair<int, int> best_move;
    for(auto checker : checkers){
        auto moves = board->possible_moves(checker[0], checker[1]);
        current_distance = get_distance(checker[0], checker[1]);
        for(auto move : moves){
            int new_distance = get_distance(move.first, move.second);
            if(current_distance - new_distance > difference){
                difference = current_distance - new_distance;
                best_checker = checker;
                best_move = move;
            }
        }
    }
    board->move(best_checker[0], best_checker[1], best_move.first, best_move.second);
    x1 = best_checker[0];
    y1 = best_checker[1];
    x2 = best_move.first;
    y2 = best_move.second;
    //Update checkers
    checkers.clear();
    get_checkers();

}

int Bot::get_x1(){
    return x1;
}

int Bot::get_y1(){
    return y1;
}

int Bot::get_x2(){
    return x2;
}

int Bot::get_y2(){
    return y2;
}

