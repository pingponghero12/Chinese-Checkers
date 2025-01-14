#include <iostream>
#include <sstream>

#include "server_controller.hpp"
#include "server.hpp"

ServerController::ServerController(Server* server_ptr) : server(server_ptr) {
    initialize_commands();
}

/*
std::vector<int> parse_message_to_vi(const std::string& message) {
    std::istringstream iss(message);
    std::vector<int> args;
    int value;
    while (iss >> value) {
        args.push_back(value);
    }
    return args;
}
*/

std::vector<int> ServerController::parse_message_to_vi(const std::string& message) {
    std::vector<int> args;
    std::istringstream iss(message);
    std::string token;

    // Split into tokens and try to parse each one
    while (iss >> token) {
        try {
            size_t pos = 0;
            int value = std::stoi(token, &pos);
            
            // Check if the entire token was consumed (no trailing chars)
            if (pos == token.length()) {
                args.push_back(value);
            }
        } catch (const std::exception& e) {
            // Invalid number format - ignore and continue
            continue;
        }
    }

    return args;
}

bool ServerController::is_in_game(int client_id) const {
    std::lock_guard<std::mutex> lock(status_mutex);

    if (player_status.at(client_id) == -1) return false;
    return true;
}

void ServerController::parse_call(const std::string& message, int client_id) {
    std::vector<int> args = parse_message_to_vi(message);

    if (args.empty()) {
        return;
    }

    int command_id = args[0];
    args.erase(args.begin());

    std::array<int, 3> main_cmds = {1, 2, 3};

    bool client_in_game = is_in_game(client_id);
    bool is_main_command = std::find(std::begin(main_cmds), std::end(main_cmds), command_id) != std::end(main_cmds);

    if (client_in_game && is_main_command) {
        send_call("You are currently in game, you need to exit to use that command.\n", client_id);
        return;
    }
    else if (!client_in_game && !is_main_command) {
        send_call("You are not in game, you need to join game to use that command.\n", client_id);
        return;
    }

    auto it = command_registry_.find(command_id);
    if (it != command_registry_.end()) {
        it->second->execute(args, client_id);
    }
    else {
        std::cerr << "Command not found: " << command_id << std::endl;
        send_call("Command not found.\n", client_id);
    }
}

void ServerController::send_call(const std::string& message, int client_id) {
    server->send_message(message, client_id);
}

void ServerController::add_game(int id, const Game& game) {
    std::lock_guard<std::mutex> lock(games_mutex);
    current_games[id] = game;
}

void ServerController::delete_game(int game_id) {
    // no lock to avoid deadlock
    current_games.erase(game_id);
}

std::vector<Game> ServerController::game_list() const {
    std::lock_guard<std::mutex> lock(games_mutex);
    std::vector<Game> games;
    for (const auto& pair : current_games) {
        games.push_back(pair.second);
    }
    return games;
}

void ServerController::update_player_status(int client_id, int status) {
    std::lock_guard<std::mutex> lock(status_mutex);
    player_status[client_id] = status;
}

int ServerController::get_player_status(int client_id) const {
    std::lock_guard<std::mutex> lock(status_mutex);
    return player_status.at(client_id);
}
