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

class ServerController;
class Game {
public:
    Game() {}
    Game(int game_type, int client_id, ServerController* controller_ptr);
    ~Game() = default;

    int get_id();
    int get_game_type();
    int get_players();

    void add_player(int client_id);
    void remove_player(int client_id);

    void game_logic();
    void move(int client_id, int x, int y);
    void end();
private:
    std::vector<int> players;
    int id;
    int game_type;
    ServerController* controller;
};

#endif // __GAME__
