#include <iostream>
#include "board.hpp"
#include <vector>


//Possible to reduce the number of variables
Board::Board(int board_size) : size(board_size), 
  first_row_pins(board_size - 1), 
  HEIGHT(2 * size - 1 + 2 * first_row_pins), 
  WIDTH((size + 2 * first_row_pins - 1) * 2),
  Fileds(HEIGHT, std::vector<char>(WIDTH, ' ')),
  middle(WIDTH/2),
  left_middle(first_row_pins-1),
  right_middle(WIDTH - left_middle) {}


void Board::fill_empty_space(){
    for(int i = 0; i <= HEIGHT-1; i++){ 

        for(int j =0; j <=WIDTH; j++){
            Fileds[i][j]=' ';
        }
    }
}


void Board::fill_top_triangle(char x){
    
    for(int i =0; i<= first_row_pins -1; i++){   

        for(int j = middle-i; j<=middle+i; j=j+2){
            
            Fileds[i][j] = x;

        }
    }
}

void Board::fill_bottom_triangle(char x){
    
    for(int i = 3*first_row_pins+1; i<= 4*first_row_pins; i++){ 
        
        for(int j = middle+i-(HEIGHT-1); j<=middle-i+(HEIGHT-1); j=j+2){ 
            
            Fileds[i][j]=x;

        }
    }
}

void Board::fill_top_left(char x){

    for(int i=first_row_pins; i<= 2*first_row_pins-1;i++){

        for(int j=left_middle+i-(2*first_row_pins-1); j<=left_middle-i+(2*first_row_pins-1); j=j+2){ 
            
            Fileds[i][j] = x;
        
        }
    }
}

void Board::fill_top_right(char x){

    for(int i=first_row_pins; i<=2*first_row_pins-1;i++){

        for(int j= right_middle+i - (2*first_row_pins-1); j<=right_middle-i+(2*first_row_pins-1); j=j+2){ 
            
            Fileds[i][j]= x;
        
        }
    }
}

void Board::fill_bot_left(char x){

    for(int i = 2*first_row_pins+1; i <= 3*first_row_pins;i++){

        for(int j=left_middle-i+ (2*first_row_pins+1); j <= left_middle +i - (2*first_row_pins+1); j=j+2){ 
            
            Fileds[i][j]= x;
    
        }
    }

}

void Board::fill_bot_right(char x){

    for(int i=2*first_row_pins+1; i<=3*first_row_pins ;i++){

        for(int j=right_middle-i+ (2*first_row_pins+1); j<=right_middle+i- (2*first_row_pins+1); j=j+2){ 
            
            Fileds[i][j] = x;
        
        }
    }
}

void Board::fill_hexagon(){

    //Fill the middle hexagon with 0's
    for(int i = first_row_pins; i <= 2*first_row_pins; i++){     
        for(int j=middle-i; j<=middle+i; j=j+2){ 
            
            Fileds[i][j]='0';
        
        }
    }
    for(int i = 2*first_row_pins; i <= 3*first_row_pins;i++){  
        for(int j = middle + i - (HEIGHT - 1); j<=middle - i + (HEIGHT - 1); j=j+2){ 
            
            Fileds[i][j]='0';
        }
    }
}


void Board::setup_board(int num_players){

fill_empty_space();
fill_hexagon();

switch(num_players){

    case 2:

        fill_top_triangle('1');
        fill_bottom_triangle('2');
        fill_bot_right('0');
        fill_bot_left('0');
        fill_top_left('0');
        fill_top_right('0');

        break;

    case 3:
    //TODO: Confirm with the rules
        fill_top_triangle('1');
        fill_bottom_triangle('1');
        fill_bot_right('2');
        fill_bot_left('3');
        fill_top_left('2');
        fill_top_right('3');

        break;

    case 4:
    //TODO: Confirm with the rules
        fill_top_triangle('0');
        fill_bottom_triangle('0');
        fill_bot_right('1');
        fill_bot_left('2');
        fill_top_left('3');
        fill_top_right('4');

        break;

    case 6:

        fill_top_triangle('1');
        fill_bottom_triangle('2');
        fill_bot_right('3');
        fill_bot_left('4');
        fill_top_left('5');
        fill_top_right('6');

        break;

    default:
    //Impelment default case
        std::cout << "Incorrect number of players" << std::endl;
        break;
}

}

void Board::move_checker(){
    //Implement method
}

bool Board::check_legality(){
    return true;
}

void Board::showBoard(){

    std::cout<<"     ";

    for(int i= 0; i<=26; i++){
        std::cout<<Alphabet[i]<<"  ";
    }

    std::cout<<std::endl;

    for(int i = 0; i<=(HEIGHT-1); i++){

        std::cout<<i+11<<".  ";

        for(int j =0; j <=(WIDTH); j++){

            std::cout<<Fileds[i][j]<<" ";
            
        }
        std::cout<<std::endl;
    }
}



