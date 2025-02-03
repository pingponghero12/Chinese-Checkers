#include "game.hpp"
#include "server_controller.hpp"
#include "db_connector.hpp"
#include "board.hpp"
#include "bot.hpp"
#include "standard_board.hpp"
#include "fast_board.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

Game::Game(int number_of_players, int db_id, int board, int client_id, ServerController* controller_ptr, int with_bot) : id(client_id), db_id(db_id), game_type(number_of_players), board_type(board), with_bot(with_bot), controller(controller_ptr) {
    players.push_back(client_id);
    move_id = 0;

    if (board_type == 0) {
        board_obj = std::shared_ptr<Board>(new Standard_Board(5));
    }
    if (board_type == 1) {
        board_obj = std::shared_ptr<Board>(new Fast_Board(5));
    }
    board_obj->setup_board(number_of_players);

    if (with_bot != 0) {
        number_of_bots = number_of_players - 1;

        for(int i = 0; i < number_of_bots; i++) {
            Bot temp_bot(board_obj);
            bots.push_back(temp_bot);
        }
    }

    for(int i = 0; i < number_of_bots; i++) {
        bots[i].set_color(i + 2);
        bots[i].get_checkers();
        bots[i].set_destination();
    }
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

void Game::move(int x1, int y1, int x2, int y2) {
    std::string out =  "move " + std::to_string(x1) + " " + std::to_string(y1) + " " + std::to_string(x2) + " " + std::to_string(y2);

    controller->dbconn->insert_move(db_id, move_id, x1, y1, x2, y2);

    for (const auto& client_id : players) {
        controller->send_call(out, client_id);
    }

    // Update move id for database
    move_id++;
    if (with_bot == 1) {
        move_bot();
    }
}

//Call this function when initializing the game
void Game::move_bot() {
    if(move_id % game_type != 0) {
        std::cout << "CHUJNIGGER1" << std::endl;
        int bot_to_move = (move_id % game_type) - 1;

        std::cout << "CHUJNIGGER2" << std::endl;
        bots[bot_to_move].make_move();
        move(bots[bot_to_move].get_x1(), bots[bot_to_move].get_y1(), bots[bot_to_move].get_x2(), bots[bot_to_move].get_y2());
        std::cout << "CHUJNIGGER3" << std::endl;
    }
}
