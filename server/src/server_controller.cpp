#include <iostream>
#include <sstream>

#include "server_controller.hpp"
#include "server.hpp"
#include "secrets.hpp"

/**
 * @brief Construct a new Server Controller:: Server Controller object
 * 
 * The constructor initializes the ServerController and maps the commands
 * to the command registry based on the provided server pointer.
 *
 * @param server_ptr A pointer to the Server object that this controller manages.
 */
ServerController::ServerController(Server* server_ptr) : server(server_ptr) {
    initialize_commands();

    // Definitions in secretes.hpp
    dbconn = std::unique_ptr<DbConnector>(new DbConnector(IP, USERNAME, PASSWORD));
}

/**
 * @brief Parse the message to a vector of integers
 * 
 * This function takes a string message, splits it into tokens, and attempts
 * to convert each token into an integer. Successfully parsed integers are
 * returned in a vector.
 *
 * @param message The message string to be parsed.
 * @return A vector of integers parsed from the message.
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

/**
 * @brief Check if a player is in the game
 * 
 * This function checks the player status based on the client ID. It returns
 * true if the player is in game, and false otherwise.
 *
 * @param client_id The ID of the client to check.
 * @return True if the player is in game, false otherwise.
 */
bool ServerController::is_in_game(int client_id) const {
    std::lock_guard<std::mutex> lock(status_mutex);
    if (player_status.at(client_id) == -1) return false;
    return true;
}

/**
 * @brief Parse and execute a command call from a client
 * 
 * This function takes in a message from a client and a client ID,
 * parses the message for command identifiers and arguments, checks
 * the client's status, and calls the appropriate command if valid.
 * 
 * If the client is in game and tries to execute a main command, or
 * if the client is not in game and tries to execute a non-main command,
 * an appropriate message is sent back. If the command is found, it is
 * executed; otherwise, an error message is sent.
 * 
 * @param message The message containing the command to be executed.
 * @param client_id The ID of the client making the call.
 */
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

/**
 * @brief Send a message to a client
 * 
 * This function sends a message to a specified client using the server's
 * send_message method. 
 *
 * @param message The message to be sent to the client.
 * @param client_id The ID of the client receiving the message.
 */
void ServerController::send_call(const std::string& message, int client_id) {
    server->send_message(message, client_id);
}

/**
 * @brief Add a new game to the current games list
 * 
 * This function adds a new game to the current_games collection. It locks
 * the games mutex to ensure thread safety during the addition.
 *
 * @param id The identifier for the new game.
 * @param game The Game object to be added.
 */
void ServerController::add_game(int id, const Game& game) {
    std::lock_guard<std::mutex> lock(games_mutex);
    current_games[id] = game;
}

/**
 * @brief Delete a game from the current games list
 * 
 * This function removes a game from the current_games collection based
 * on the game ID. It does not acquire a lock to avoid potential deadlocks.
 *
 * @param game_id The identifier of the game to be deleted.
 */
void ServerController::delete_game(int game_id) {
    // no lock to avoid deadlock
    current_games.erase(game_id);
}

/**
 * @brief Retrieve the list of current games
 * 
 * This function returns a vector containing the current games. It locks
 * the games mutex to ensure thread safety while retrieving the list.
 *
 * @return A vector of Game objects currently available.
 */
std::vector<Game> ServerController::game_list() const {
    std::lock_guard<std::mutex> lock(games_mutex);
    std::vector<Game> games;
    for (const auto& pair : current_games) {
        games.push_back(pair.second);
    }
    return games;
}

/**
 * @brief Update the status of a player
 * 
 * This function updates the status of a player identified by client_id.
 * It locks the status mutex to ensure thread safety while updating the status.
 *
 * @param client_id The ID of the client whose status is to be updated.
 * @param status The new status value to assign to the player.
 */
void ServerController::update_player_status(int client_id, int status) {
    std::lock_guard<std::mutex> lock(status_mutex);
    player_status[client_id] = status;
}

/**
 * @brief Get the status of a player
 * 
 * This function retrieves the status of a player identified by client_id.
 * It locks the status mutex to ensure thread safety while accessing the
 * status.
 * 
 * @param client_id The ID of the client whose status is to be retrieved.
 * @return The status of the player.
 */
int ServerController::get_player_status(int client_id) const {
    std::lock_guard<std::mutex> lock(status_mutex);
    return player_status.at(client_id);
}
