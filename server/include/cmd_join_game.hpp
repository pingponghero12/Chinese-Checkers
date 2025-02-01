#ifndef __CMDJOINGAME__
#define __CMDJOINGAME__

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_map>

#include "abstract_command.hpp"
#include "server_controller.hpp"
#include "game.hpp"

/**
 * @brief Command to join a game
 * The command adds a player to a game
 * The player is added to the game with a given id
 * 
 */

// Join type
const int CURRENT = 0;
const int HISTORY = 1;

class CmdJoinGame : public AbstractCommand {
public:
    CmdJoinGame(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        if (args.size() != 2) {
            std::cerr << "Error: history/current id" << std::endl;
            return;
        }

        if (args[0] == CURRENT) {
            int game_id = args[1];
            controller.update_player_status(client_id, game_id);

            int players;
            int id;
            int board;
            {
                std::lock_guard<std::mutex> lock(controller.games_mutex);
                auto it = controller.current_games.find(game_id);
                if (it != controller.current_games.end()) {
                    it->second.add_player(client_id);
                    players = it->second.get_game_type();
                    id = it->second.get_players();
                    board = it->second.get_board();
                } else {
                    std::cerr << "Error: Game not found" << std::endl;
                    controller.send_call("Error: Game not found\n", client_id);
                    return;
                }
            }

            controller.send_call("joined " + std::to_string(players) + " " + std::to_string(id) + " " + std::to_string(board), client_id);
        }
        else if (args[0] == HISTORY) {
            int game_id = args[1];
            controller.update_player_status(client_id, game_id);

            int players;
            int id;
            int board;

            std::vector<int> game_info = controller.dbconn->get_game(game_id);
            players = game_info[2];
            id = game_info[0];
            board = game_info[1];

            controller.send_call("joined " + std::to_string(players) + " " + std::to_string(id) + " " + std::to_string(board), client_id);
        }
    }

private:
    ServerController& controller;
};

#endif // __CMDJOINGAME__
