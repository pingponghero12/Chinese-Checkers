#include "server_controller.hpp"
#include "server.hpp"

#include "cmd_add_game.hpp"
#include "cmd_list_games.hpp"
#include "cmd_join_game.hpp"
#include "cmd_exit_game.hpp"

ServerController::ServerController(Server* server_ptr) : server(server_ptr) {
    server = server;
    initialize_commands();
}

void ServerController::initialize_commands() {
    command_registry_[1] = std::make_unique<CmdAddGame>(*this);
    command_registry_[2] = std::make_unique<CmdListGames>(*this);
    command_registry_[3] = std::make_unique<CmdJoinGame>(*this);
    command_registry_[5] = std::make_unique<CmdExitGame>(*this);
}

std::vector<int> parse_stvi(const std::string& message) {
    std::istringstream iss(message);
    std::vector<int> args;
    int value;
    while (iss >> value) {
        args.push_back(value);
    }
    return args;
}

void ServerController::parse_call(const std::string& message, int client_number) {
    std::vector<int> args = parse_stvi(message);

    if (args.empty()) {
        return;
    }


    int command_index = args[0];
    args.erase(args.begin());

    std::cout << "execute " <<args[0]<< std::endl;
    auto it = command_registry_.find(command_index);
    if (it != command_registry_.end()) {
        it->second->execute(args, client_number);
    }
    else {
        std::cerr << "Command not found: " << command_index << std::endl;
    }
}

void ServerController::send_call(const std::string& message, int client_number) {
    server->send_message(message, client_number);
}

void ServerController::add_game(int id, const Game& game) {
    std::lock_guard<std::mutex> lock(games_mutex);
    current_games[id] = game;
}

void ServerController::delete_game(int game_id) {
    // no lock to avoid deadlock
    current_games.erase(game_id);
}

std::vector<Game> ServerController::game_list() {
    std::lock_guard<std::mutex> lock(games_mutex);
    std::vector<Game> games;
    for (const auto& pair : current_games) {
        games.push_back(pair.second);
    }
    return games;
}

void ServerController::update_player_status(int client_number, int status) {
    std::lock_guard<std::mutex> lock(status_mutex);
    player_status[client_number] = status;
}

int ServerController::get_player_status(int client_number) {
    std::lock_guard<std::mutex> lock(status_mutex);
    return player_status[client_number];
}
