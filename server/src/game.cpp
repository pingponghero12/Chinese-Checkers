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
