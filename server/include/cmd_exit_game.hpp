#ifndef __CMDEXITGAME__
#define __CMDEXITGAME__

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
 * @brief Command to exit a game
 * The command removes a player from a game
 * If player is not in a game, the function prints an error message
 * After sucessfully exiting the game, the function prints a confirmation message
 */
class CmdExitGame : public AbstractCommand {
public:
    CmdExitGame(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        if (args.size() != 0) {
            std::cerr << "Error: function takes no args" << std::endl;
            return;
        }

        int game_id = controller.get_player_status(client_id);
        controller.update_player_status(client_id, -1);

        {
            std::lock_guard<std::mutex> lock(controller.games_mutex);
            auto it = controller.current_games.find(game_id);
            if (it != controller.current_games.end()) {
                it->second.remove_player(client_id);
            } else {
                // This is fucking shit
                std::cerr << "Error: Game not found" << std::endl;
            }
        }

        controller.send_call("exited", client_id);
    }

private:
    ServerController& controller;
};

#endif // __CMDEXITGAME__
