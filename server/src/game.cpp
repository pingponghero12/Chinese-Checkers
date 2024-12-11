#include "game.hpp"

Game::Game(int number_of_players, int client_number, ServerController* controller_ptr) : id(client_number), game_type(number_of_players), controller(controller_ptr) {
    players.push_back(client_number);
}

int Game::get_id() {
    return id;
}

int Game::get_game_type() {
    return game_type;
}

int Game::get_players() {
    return players.size();
}

void Game::add_player(int client_number) {
    players.push_back(client_number);
}

void Game::remove_player(int client_number) {
    auto it = std::find(players.begin(), players.end(), client_number);
    if (it == players.end()) {
        std::cerr << "Error: Player not found in the game" << std::endl;
        return;
    }

    if (id == client_number) {
        end();
    }
    if (it != players.end()) {
        players.erase(it);
    }
}

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
