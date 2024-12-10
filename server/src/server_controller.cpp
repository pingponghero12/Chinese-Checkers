#include "server_controller.hpp"
#include "server.hpp"

#include "cmd_add_game.hpp"
#include "cmd_list_games.hpp"

ServerController::ServerController(Server* server) : server(server) {
    server = server;
    initialize_commands();
}

void ServerController::initialize_commands() {
    command_registry_[1] = std::make_unique<CmdAddGame>(*this);
    command_registry_[2] = std::make_unique<CmdListGames>(*this);
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

void ServerController::add_game(const Game& game) {
    current_games.push_back(game);
}

void ServerController::delete_game(int game_id) {
    current_games.erase(current_games.begin() + game_id);
}

std::vector<Game> ServerController::game_list() {
    return current_games;
}

