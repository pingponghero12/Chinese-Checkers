#ifndef __GAME__
#define __GAME__

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <memory>
#include <sstream>

/**
 * @brief Game class
 * The class represents a game on the server
 * The game is initialized with a game type and a host
 * Players can be added and removed from the game
 * The game logic is executed in a separate thread
 */
class ServerController;
class Game {
public:
    Game() {}
    Game(int game_type, int db_id, int board, int client_id, ServerController* controller_ptr);
    ~Game() = default;

    int get_id();
    int get_game_type();
    int get_players();
    int get_board();

    void add_player(int client_id);
    void remove_player(int client_id);

    void game_logic();
    void move(int client_id, int x1, int y1, int x2, int y2);
    void end();
private:
    std::vector<int> players;
    int id;
    int db_id;
    int game_type;
    int board_type;
    int move_id;
    ServerController* controller;
};

#endif // __GAME__
