#include "game.hpp"

Game::Game(int number_of_players, int client_number) {
    id = client_number;
    game_type = number_of_players;
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
    if (it != players.end()) {
        players.erase(it);
    }
}
