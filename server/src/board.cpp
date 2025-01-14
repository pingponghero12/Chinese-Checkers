#include <iostream>
#include "board.hpp"
#include <vector>



    Board::Board(int board_size)
    : size(board_size), 
      Fileds((2 * size - 1 + 2 * (size - 1)), std::vector<char>((6*(size-1))+1, ' ')) {}   


    void Board::fill_empty_space(){
        for(int i = 0; i <= 4*(size -1); i++){ 

            for(int j =0; j <= (6*(size-1)); j++){
                Fileds[i][j]=' ';
            }
        }
    }
    

    void Board::fill_top_triangle(char x){
        
        for(int i =0; i<= (size - 1) -1; i++){   

            for(int j = (3*(size-1))-i; j<=(3*(size-1))+i; j=j+2){
                
                Fileds[i][j] = x;

            }
        }
    }

    void Board::fill_bottom_triangle(char x){
        
        for(int i = 3*(size - 1)+1; i<= 4*(size - 1); i++){ 
            
            for(int j = i - size +1; j<= 7*(size -1) - i; j=j+2){ 
                
                Fileds[i][j]=x;

            }
        }
    }

    void Board::fill_top_left(char x){

        for(int i=(size - 1); i<= 2*(size - 1)-1;i++){

            for(int j = i - size +1; j <= 3*size - 5 -i; j=j+2){ 
                
                Fileds[i][j] = x;
            
            }
        }
    }

    void Board::fill_top_right(char x){

        for(int i=(size - 1); i<=2*(size - 1)-1 ;i++){

            for(int j= 3*size -1 + i; j<= 7*(size -1) - i; j=j+2){ 
                
                Fileds[i][j]= x;
            
            }
        }
    }

    void Board::fill_bot_left(char x){

        for(int i = 2*(size - 1)+1; i <= 3*(size - 1);i++){

            for(int j= 3*(size - 1) - i; j <= i - size -1; j=j+2){ 
                
                Fileds[i][j]= x;
        
            }
        }

    }

    void Board::fill_bot_right(char x){

        for(int i=2*(size - 1)+1; i<=3*(size - 1) ;i++){

            for(int j= 7* size - 5 -i; j<=3 * size -3 +i; j=j+2){ 
                
                Fileds[i][j] = x;
            
            }
        }
    }

    void Board::fill_hexagon(){

        //Fill the middle hexagon with 0's
        for(int i = (size - 1); i <= 2*(size - 1); i++){     
            for(int j=(3*(size-1))-i; j<=(3*(size-1))+i; j=j+2){ 
                
                Fileds[i][j]='0';
            
            }
        }
        for(int i = 2*(size - 1); i <= 3*(size - 1);i++){  
            for(int j = i + 1 - size; j<= 7*(size-1) - i; j=j+2){ 
                
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
            fill_bottom_triangle('4');
            fill_bot_right('3');
            fill_bot_left('5');
            fill_top_left('6');
            fill_top_right('2');

            break;

        default:
        //Impelment default case
            std::cout << "Incorrect number of players" << std::endl;
            break;
    }

    }

    void Board::move_checker(int x1, int y1, int x2, int y2){
        if(Fileds[x1][y1] == '0' || Fileds[x1][y1] == ' '){
            std::cout << "No checker at this position" << std::endl;
            return;
        }
        if(Fileds[x2][y2] != '0'){
            std::cout << "There is already a checker at this position" << std::endl;
            return;
        }
        Fileds[x2][y2] = Fileds[x1][y1];
        Fileds[x1][y1] = '0';
    }

    bool Board::check_legality(){
        return true;
    }

    void Board::possible_shifts(int x, int y){
        if(Fileds[x][y] == '0' || Fileds[x][y] == ' '){
            std::cout << "No checker at this position" << std::endl;
            return;
        }
        if(Fileds[x][y-2]=='0'){
            std::cout << "Possible move to " << x<< " " << y-2  << std::endl;
        }
        if(Fileds[x][y+2]=='0'){
            std::cout << "Possible move to " << x << " " << y+2 << std::endl;
        }
        if(Fileds[x-1][y-1]=='0'){
            std::cout << "Possible move to " << x-1 << " " << y-1 << std::endl;
        }
        if(Fileds[x+1][y+1]=='0'){
            std::cout << "Possible move to " << x+1 << " " << y+1 << std::endl;
        }
        if(Fileds[x-1][y+1]=='0'){
            std::cout << "Possible move to " << x-1 << " " << y+1 << std::endl;
        }
        if(Fileds[x+1][y-1]=='0'){
            std::cout << "Possible move to " << x+1 << " " << y-1 << std::endl;
        }

    }

    void Board::possible_jumps(int x, int y){
        
        
        if(Fileds[x-1][y-1]!='0' && Fileds[x-2][y-2]=='0'){
            std::cout << "Possible jump to " << x-2 << " " << y-2 << std::endl;
            next_jump(x-2, y-2, x, y);
        }
        if(Fileds[x+1][y+1]!='0' && Fileds[x+2][y+2]=='0'){
            std::cout << "Possible jump to " << x+2 << " " << y+2 << std::endl;
            next_jump(x+2, y+2, x, y);
        }
        if(Fileds[x-1][y+1]!='0' && Fileds[x-2][y+2]=='0'){
            std::cout << "Possible jump to " << x-2 << " " << y+2 << std::endl;
            next_jump(x-2, y+2, x, y);
        }
        if(Fileds[x+1][y-1]!='0' && Fileds[x+2][y-2]=='0'){
            std::cout << "Possible jump to " << x+2 << " " << y-2 << std::endl;
            next_jump(x+2, y-2, x, y);
        }
    }

    //x and y are the point to calculate the jump from, a and b are the point from which previous jump was made
    //This function has to guarrentee that you can't jump back to the same place
    void Board::next_jump(int x, int y, int a, int b){
        if(Fileds[x-1][y-1]!='0' && Fileds[x-2][y-2]=='0' && (x-2 != a || y-2 != b)){
            std::cout << "Possible jump to " << x-2 << " " << y-2 << std::endl;
            next_jump(x-2, y-2, x, y);
        }
        if(Fileds[x+1][y+1]!='0' && Fileds[x+2][y+2]=='0' && (x+2 != a || y+2 != b)){
            std::cout << "Possible jump to " << x+2 << " " << y+2 << std::endl;
            next_jump(x+2, y+2, x, y);
        }
        if(Fileds[x-1][y+1]!='0' && Fileds[x-2][y+2]=='0' && (x-2 != a || y+2 != b)){
            std::cout << "Possible jump to " << x-2 << " " << y+2 << std::endl;
            next_jump(x-2, y+2, x, y);
        }
        if(Fileds[x+1][y-1]!='0' && Fileds[x+2][y-2]=='0' && (x+2 != a || y-2 != b)){
            std::cout << "Possible jump to " << x+2 << " " << y-2 << std::endl;
            next_jump(x+2, y-2, x, y);
        }
    }

    void Board::showBoard(){


        std::cout<<std::endl;

        for(int i = 0; i<=(4*(size-1)); i++){

            std::cout<<i+11<<".  ";

            for(int j =0; j <=((6*(size-1))); j++){

                std::cout<<Fileds[i][j]<<" ";
                
            }
            std::cout<<std::endl;
        }
    }
    


