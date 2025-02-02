#ifndef __GAME__
#define __GAME__

#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>

#include <bot.hpp>

class ServerController;
class Board;

/**
 * @brief Game class
 * The class represents a game on the server
 * The game is initialized with a game type and a host
 * Players can be added and removed from the game
 * The game logic is executed in a separate thread
 */

class Game {
public:
    Game() {}
    Game(int game_type, int db_id, int board, int client_id, ServerController* controller_ptr, int with_bot);
    ~Game() = default;

    int get_id();
    int get_game_type();
    int get_players();
    int get_board();

    void add_player(int client_id);
    void remove_player(int client_id);

    void game_logic();
    void move(int x1, int y1, int x2, int y2);
    void end();
private:
    std::vector<int> players;
    int id;
    int db_id;
    int game_type;
    int board_type;
    int move_id;
    int number_of_bots;
    ServerController* controller;

    std::vector<Bot> bots;
    std::shared_ptr<Board> board_obj;
};

#endif // __GAME__
