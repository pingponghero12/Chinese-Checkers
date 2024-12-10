#include "server_controller.hpp"
#include "server.hpp"

#include "cmd_add_game.hpp"
#include "cmd_list_games.hpp"
#include "cmd_join_game.hpp"

ServerController::ServerController(Server* server) : server(server) {
    server = server;
    initialize_commands();
}

void ServerController::initialize_commands() {
    command_registry_[1] = std::make_unique<CmdAddGame>(*this);
    command_registry_[2] = std::make_unique<CmdListGames>(*this);
    command_registry_[3] = std::make_unique<CmdJoinGame>(*this);
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
    current_games[id] = game;
}

void ServerController::delete_game(int game_id) {
    current_games.erase(game_id);
}

std::vector<Game> ServerController::game_list() {
    std::vector<Game> games;
    for (const auto& pair : current_games) {
        games.push_back(pair.second);
    }
    return games;
}

void ServerController::update_player_status(int client_number, int status) {
    player_status[client_number] = status;
}

void ServerController::add_player_to_game(int client_number, int id) {
    current_games[id].add_player(client_number);
}
