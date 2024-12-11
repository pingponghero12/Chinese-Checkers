#include "server_controller.hpp"
#include "server.hpp"

#include "cmd_add_game.hpp"
#include "cmd_list_games.hpp"
#include "cmd_join_game.hpp"
#include "cmd_exit_game.hpp"
#include "cmd_move.hpp"

ServerController::ServerController(Server* server_ptr) : server(server_ptr) {
    server = server;
    initialize_commands();
}

void ServerController::initialize_commands() {
    command_registry_[1] = std::make_unique<CmdAddGame>(*this);
    command_registry_[2] = std::make_unique<CmdListGames>(*this);
    command_registry_[3] = std::make_unique<CmdJoinGame>(*this);
    command_registry_[4] = std::make_unique<CmdMove>(*this);
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

bool ServerController::in_lobby(int client_number) {
    std::lock_guard<std::mutex> lock(status_mutex);

    if (player_status.at(client_number) == -1) return false;
    return true;
}

void ServerController::parse_call(const std::string& message, int client_number) {
    std::vector<int> args = parse_stvi(message);

    if (args.empty()) {
        return;
    }

    int command_index = args[0];
    args.erase(args.begin());

    std::array<int, 3> main_cmds = {1, 2, 3};

    // Check if call illegal
    auto itt = std::find(std::begin(main_cmds), std::end(main_cmds), command_index);
    if (in_lobby(client_number) && itt != std::end(main_cmds)) {
        send_call("You are in lobby, you need to exit to use that command.\n", client_number);
        return;
    }
    else if (!in_lobby(client_number) && itt == std::end(main_cmds)) {
        send_call("You are in lobby, you need to exit to use that command.\n", client_number);
        return;
    }

    auto it = command_registry_.find(command_index);
    if (it != command_registry_.end()) {
        it->second->execute(args, client_number);
    }
    else {
        std::cerr << "Command not found: " << command_index << std::endl;
        send_call("Command not found.\n", client_number);
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
