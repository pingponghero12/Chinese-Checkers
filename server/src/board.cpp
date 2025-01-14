#include <iostream>
#include "board.hpp"
#include <vector>
/**
 * @brief Construct a new Board:: Board object
 * 
 * @param board_size 
 * Board is an abstract class that represents the board of the game.
 * It is initialized with a size and a 2D vector of characters.
 * The size for standard board is 5.
 * 
 */

Board::Board(int board_size)
    : size(board_size), 
      Fileds((2 * size - 1 + 2 * (size - 1)), std::vector<char>((6 * (size - 1)) + 1, ' ')) {}
/**
 * @brief Fills the empty space of the board with spaces
 * 
 */
void Board::fill_empty_space() {
    for (int i = 0; i <= 4 * (size - 1); i++) {
        for (int j = 0; j <= (6 * (size - 1)); j++) {
            Fileds[i][j] = ' ';
        }
    }
}

/**
 * @brief Fills the top triangle of the board with a given character
 * Used to setup the board for player playing in the top triangle
 * @param x 
 */

void Board::fill_top_triangle(char x) {
    for (int i = 0; i <= (size - 1) - 1; i++) {
        for (int j = (3 * (size - 1)) - i; j <= (3 * (size - 1)) + i; j = j + 2) {
            Fileds[i][j] = x;
        }
    }
}

/**
 * @brief Fills the bottom triangle of the board with a given character
 * Used to setup the board for player playing in the bottom triangle
 * @param x 
 */
void Board::fill_bottom_triangle(char x) {
    for (int i = 3 * (size - 1) + 1; i <= 4 * (size - 1); i++) {
        for (int j = i - size + 1; j <= 7 * (size - 1) - i; j = j + 2) {
            Fileds[i][j] = x;
        }
    }
}

/**
 * @brief Fills the top left corner of the board with a given character
 * Used to setup the board for player playing in the top left corner
 * @param x 
 */
void Board::fill_top_left(char x) {
    for (int i = (size - 1); i <= 2 * (size - 1) - 1; i++) {
        for (int j = i - size + 1; j <= 3 * size - 5 - i; j = j + 2) {
            Fileds[i][j] = x;
        }
    }
}

/**
 * @brief Fills the top right corner of the board with a given character
 * Used to setup the board for player playing in the top right corner
 * @param x 
 */
void Board::fill_top_right(char x) {
    for (int i = (size - 1); i <= 2 * (size - 1) - 1; i++) {
        for (int j = 3 * size - 1 + i; j <= 7 * (size - 1) - i; j = j + 2) {
            Fileds[i][j] = x;
        }
    }
}

/**
 * @brief Fills the bottom left corner of the board with a given character
 * Used to setup the board for player playing in the bottom left corner
 * @param x 
 */
void Board::fill_bot_left(char x) {
    for (int i = 2 * (size - 1) + 1; i <= 3 * (size - 1); i++) {
        for (int j = 3 * (size - 1) - i; j <= i - size - 1; j = j + 2) {
            Fileds[i][j] = x;
        }
    }
}

/**
 * @brief Fills the bottom right corner of the board with a given character
 * Used to setup the board for player playing in the bottom right corner
 * @param x 
 */
void Board::fill_bot_right(char x) {
    for (int i = 2 * (size - 1) + 1; i <= 3 * (size - 1); i++) {
        for (int j = 7 * size - 5 - i; j <= 3 * size - 3 + i; j = j + 2) {
            Fileds[i][j] = x;
        }
    }
}

/**
 * @brief Fills the hexagon of the board with zeros
 * The hexagon is the main part of the board where the game is played
 * Players are only able to move to fields filled with zeros
 */
void Board::fill_hexagon() {
    for (int i = (size - 1); i <= 2 * (size - 1); i++) {
        for (int j = (3 * (size - 1)) - i; j <= (3 * (size - 1)) + i; j = j + 2) {
            Fileds[i][j] = '0';
        }
    }
    for (int i = 2 * (size - 1); i <= 3 * (size - 1); i++) {
        for (int j = i + 1 - size; j <= 7 * (size - 1) - i; j = j + 2) {
            Fileds[i][j] = '0';
        }
    }
}

/**
 * @brief Sets up the board for a given number of players
 * The function fills the board with pieces for each player
 * The number of players is passed as an argument
 * @param num_players 
 */
void Board::setup_board(int num_players) {
    fill_empty_space();
    fill_hexagon();
    
    //Fill specific triangles and corners based on the number of players
    switch (num_players) {
        case 2:
            fill_top_triangle('1');
            fill_bottom_triangle('2');
            fill_bot_right('0');
            fill_bot_left('0');
            fill_top_left('0');
            fill_top_right('0');
            break;
        case 3:
            fill_top_triangle('1');
            fill_bottom_triangle('1');
            fill_bot_right('2');
            fill_bot_left('3');
            fill_top_left('2');
            fill_top_right('3');
            break;
        case 4:
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
            std::cout << "Incorrect number of players" << std::endl;
            break;
    }
}

/**
 * @brief Moves a checker from one position to another
 * The function checks if the move is legal
 * If the move is legal, the function moves the checker
 * If the move is illegal, the function prints an error message
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 */
void Board::move(int x1, int y1, int x2, int y2) {
    if (Fileds[x1][y1] == '0' || Fileds[x1][y1] == ' ') {
        std::cout << "No checker at this position" << std::endl;
        return;
    }
    if (Fileds[x2][y2] != '0') {
        std::cout << "There is already a checker at this position" << std::endl;
        return;
    }
    Fileds[x2][y2] = Fileds[x1][y1];
    Fileds[x1][y1] = '0';
}

/**
 * @brief Shows the board
 * The function prints the board to the console
 * Mainly used for debugging
 */
void Board::showBoard() {
    std::cout << std::endl;
    for (int i = 0; i <= (4 * (size - 1)); i++) {
        std::cout << i + 11 << ".  ";
        for (int j = 0; j <= (6 * (size - 1)); j++) {
            std::cout << Fileds[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Returns the fields of the board
 * The function returns the 2D vector of characters representing the board
 * @return std::vector<std::vector<char>> 
 */
std::vector<std::vector<char>> Board::getFields() const {
    return Fileds;
}

/**
 * @brief Returns the characters at a given position
 * If the position is out of bounds, the function returns a space indicating a prohibited field
 * 
 */
char Board::get_Field(int x, int y) const {
    if (x < 0 || x > 4 * (size - 1) || y < 0 || y > 6 * (size - 1)) {
        return ' ';
    }
    return Fileds[x][y];
}

/**
 * @brief Sets the character at a given position
 * 
 */
void Board::set_Field(int x, int y, char value) {
    Fileds[x][y] = value;
}

/**
 * @brief Returns the size of the board
 * 
 */
int Board::get_size() const {
    return size;
}
