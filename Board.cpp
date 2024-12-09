#ifndef Board_H
#define Board_H


#include <iostream>

class Board{
private:

    //Size can be later used as a construct for bigger boards
    //Size - number of fields at side of the central hexagon (default 5)
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
    Board(int board_size){
        size = board_size;
    }

    

    void fill_top_triangle(char x){
        
        for(int i =0; i<= first_row_pins -1; i++){   

            for(int j = middle-i; j<=middle+i; j=j+2){
                
                Fileds[i][j] = x;

            }
        }

    }

    void fill_bottom_triangle(char x){
        
        for(int i = 3*first_row_pins+1; i<= 4*first_row_pins; i++){ 
            
            for(int j = middle+i-(HEIGHT-1); j<=middle-i+(HEIGHT-1); j=j+2){ 
                
                Fileds[i][j]=x;

            }
        }
    }

    void fill_top_left(char x){

        for(int i=first_row_pins; i<= 2*first_row_pins-1;i++){

            for(int j=left_middle+i-(2*first_row_pins-1); j<=left_middle-i+(2*first_row_pins-1); j=j+2){ 
                
                Fileds[i][j] = x;
            
            }
        }
    }

    void fill_top_right(char x){

        for(int i=first_row_pins; i<=2*first_row_pins-1;i++){

            for(int j= right_middle+i - (2*first_row_pins-1); j<=right_middle-i+(2*first_row_pins-1); j=j+2){ 
                
                Fileds[i][j]= x;
            
            }
        }
    }

    void fill_bot_left(char x){

        for(int i = 2*first_row_pins+1; i <= 3*first_row_pins;i++){

            for(int j=left_middle-i+ (2*first_row_pins+1); j <= left_middle +i - (2*first_row_pins+1); j=j+2){ 
                
                Fileds[i][j]= x;
        
            }
        }

    }

    void fill_bot_right(char x){

        for(int i=2*first_row_pins+1; i<=3*first_row_pins ;i++){

            for(int j=right_middle-i+ (2*first_row_pins+1); j<=right_middle+i- (2*first_row_pins+1); j=j+2){ 
                
                Fileds[i][j] = x;
            
            }
        }
    }



    void setup_board(int num_players){

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

    switch(num_players){

        case 2:

            fill_top_triangle(1);
            fill_bottom_triangle(2);
            fill_bot_right(0);
            fill_bot_left(0);
            fill_top_left(0);
            fill_top_right(0);

            break;

        case 3:
        //TODO: Confirm with the rules
            fill_top_triangle(1);
            fill_bottom_triangle(1);
            fill_bot_right(2);
            fill_bot_left(3);
            fill_top_left(2);
            fill_top_right(3);

            break;

        case 4:
        //TODO: Confirm with the rules
            fill_top_triangle(0);
            fill_bottom_triangle(0);
            fill_bot_right(1);
            fill_bot_left(2);
            fill_top_left(3);
            fill_top_right(4);

            break;

        case 6:

            fill_top_triangle(1);
            fill_bottom_triangle(2);
            fill_bot_right(3);
            fill_bot_left(4);
            fill_top_left(5);
            fill_top_right(6);

            break;

        default:
        //Impelment default case

    }

    }

    void move_checker(){
        //Implement method
    }

    void check_legality(){

    }

    void showBoard(){

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
    
};

#endif Board_H