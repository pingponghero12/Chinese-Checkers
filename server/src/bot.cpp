#include <iostream>
#include <vector>
#include "board.hpp"
#include "bot.hpp"
#include <cmath>

//Możliwe że będzie potrzebny osobny bot do każdej planszy

Bot::Bot(std::shared_ptr<Board> board) : board(board) {
    //Constructor for the bot
}

void Bot::set_color(char color) {
    this->color = color;
}

void Bot::get_checkers() {
    for (int i = 0; i <= 4 * (board->get_size() - 1); i++) {
        for (int j = 0; j <= 6 * (board->get_size() - 1); j++) {
            if (board->get_Field(i, j) == color) {
                std::cout << i << " " << j << std::endl;
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
            std::cout << "destination: " << destination[0].first << " " << destination[0].second << std::endl;
        }

        if(checker[0] == 16 && checker [1] == 12){
            destination.push_back(std::make_pair(0, 12));
            std::cout << "destination: " << destination[0].first << " " << destination[0].second << std::endl;
            
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
            std::cout <<"chekers: " << checker[0] << " " << checker[1]<<" cur distance "<<current_distance<< " move: " << move.first <<" "<< move.second <<" new distance: "<<new_distance << std::endl;
            if(current_distance - new_distance > difference){
                difference = current_distance - new_distance;
                best_checker = checker;
                best_move = move;
            }
        }
    }
    board->move(best_checker[0], best_checker[1], best_move.first, best_move.second);
    std::cout<<"best move: " << best_checker[0] << " " << best_checker[1] << " " << best_move.first << " " << best_move.second << std::endl;
    //Update checkers
    checkers.clear();
    get_checkers();

}

