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

class Game {
public:
    Game() {}
    Game(int game_type, int client_number);
    ~Game() = default;

    int get_id();
    int get_game_type();
    int get_players();

    void add_player(int client_number);
    void remove_player(int client_number);
private:
    std::vector<int> players;
    int id;
    int game_type;
};

#endif // __GAME__
