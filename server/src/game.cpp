#include "game.hpp"
#include "server_controller.hpp"
#include "db_connector.hpp"

Game::Game(int number_of_players, int db_id, int board, int client_id, ServerController* controller_ptr) : id(client_id), db_id(db_id), game_type(number_of_players), board_type(board), controller(controller_ptr) {
    players.push_back(client_id);
    move_id = 0;
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

int Game::get_board() {
    return board_type;
}

void Game::add_player(int client_id) {
    players.push_back(client_id);
}

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

void Game::move(int client_id, int x1, int y1, int x2, int y2) {
    std::string out =  "move " + std::to_string(x1) + " " + std::to_string(y1) + " " + std::to_string(x2) + " " + std::to_string(y2);

    controller->dbconn->insert_move(db_id, move_id, x1, y1, x2, y2);

    for (const auto& client_id : players) {
        controller->send_call(out, client_id);
    }

    // Update move id for database
    move_id++;
}
