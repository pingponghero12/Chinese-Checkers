#ifndef __SERVERCONTROLLER__
#define __SERVERCONTROLLER__

#include <vector>
#include <string>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <array>

#include "game.hpp"
#include "abstract_command.hpp"

class Server;
class Game;

class ServerController {
public:
    ServerController(Server* server_ptr);
    ~ServerController() = default;

    void parse_call(const std::string& message, int client_id);
    void send_call(const std::string& message, int client_id);

    void add_game(int id, const Game& game);
    void delete_game(int game_id);
    std::vector<Game> game_list() const;

    void update_player_status(int client_id, int status);
    int get_player_status(int client_id) const;
    std::unordered_map<int, Game> current_games;

    mutable std::mutex games_mutex;
    mutable std::mutex status_mutex;

private:
    Server* server;
    std::unordered_map<int, std::unique_ptr<AbstractCommand>> command_registry_;
    std::unordered_map<int, int> player_status; // -1 if not in game, game_id otherwise

    void initialize_commands();
    bool is_in_game(int client_id) const;
};

#endif // __SERVERCONTROLLER__
