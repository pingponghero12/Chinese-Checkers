#include "game.hpp"
#include "server_controller.hpp"

/**
 * @brief Construct a new Game:: Game object
 * 
 * @param game_type 
 * @param client_id 
 * @param controller_ptr 
 */
Game::Game(int number_of_players, int client_id, ServerController* controller_ptr) : id(client_id), game_type(number_of_players), controller(controller_ptr) {
    players.push_back(client_id);
}
/**
 * @brief Get the id object
 * 
 * @return int 
 */
int Game::get_id() {
    return id;
}

/**
 * @brief Get the game type object
 * 
 * @return int 
 */
int Game::get_game_type() {
    return game_type;
}

/**
 * @brief Get the players object
 * 
 * @return int 
 */
int Game::get_players() {
    return players.size();
}

/**
 * @brief Adds a player to the game
 * 
 * @param client_id 
 */
void Game::add_player(int client_id) {
    players.push_back(client_id);
}

/**
 * @brief Removes a player from the game
 * 
 * @param client_id 
 */
void Game::remove_player(int client_id) {
    auto it = std::find(players.begin(), players.end(), client_id);
    if (it == players.end()) {
        std::cerr << "Error: Player not found in the game" << std::endl;
        return;
    }

    if (id == client_id) {
        end();
    }
    if (it != players.end()) {
        players.erase(it);
    }
}

/**
 * @brief Game logic
 * The function is responsible for the game logic
 * The function sends a message to all players that the game has ended
 */
void Game::end() {
    for (const auto& player : players) {
        if (controller) {
            controller->send_call("Game ended\n", player);
            controller->update_player_status(player, -1);
        }
        else {
            std::cerr << "Error: Controller is null" << std::endl;
        }
    }
    
    if (controller) {
        controller->delete_game(id);
    }
    else {
        std::cerr << "Error: Controller is null" << std::endl;
    }
}

/**
 * @brief Move a checker
 * The function sends a message to all players with the move
 * @param client_id 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 */
void Game::move(int client_id, int x1, int y1, int x2, int y2) {
    std::string out =  "move," + std::to_string(x1) + "," + std::to_string(y1) + "," + std::to_string(x2) + "," + std::to_string(y2);
    for (const auto& client_id : players) {
        controller->send_call(out, client_id);
    }
}
