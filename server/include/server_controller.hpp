#ifndef __SERVERCONTROLLER__
#define __SERVERCONTROLLER__

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
#include <array>

#include "game.hpp"
#include "abstract_command.hpp"

class Server;
class Game;

class ServerController {
public:
    ServerController(Server* server_ptr);
    ~ServerController() = default;

    void parse_call(const std::string& message, int client_number);
    void send_call(const std::string& message, int client_number);

    void add_game(int id, const Game& game);
    void delete_game(int game_id);
    std::vector<Game> game_list();

    // Status =-1 if not in game, if in game =game_id
    void update_player_status(int client_number, int status);
    int get_player_status(int client_number);
    std::unordered_map<int, Game> current_games;

    std::mutex games_mutex;
    std::mutex status_mutex;

private:
    Server* server;
    std::unordered_map<int, std::unique_ptr<AbstractCommand>> command_registry_;
    // Client_id to current game or -1 for not in game
    std::unordered_map<int, int> player_status;

    void initialize_commands();
    bool in_lobby(int client_number);
};

#endif // __SERVERCONTROLLER__
