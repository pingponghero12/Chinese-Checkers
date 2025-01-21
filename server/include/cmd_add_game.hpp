#ifndef __CMDADDGAME__
#define __CMDADDGAME__

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <memory>
#include <sstream>

#include "abstract_command.hpp"
#include "server_controller.hpp"
#include "game.hpp"

/**
 * @brief Command to add a game
 * The command adds a game to the server
 * The game is created with a given number of players
 * The game is added to the server controller
 * The player who added the game is set as the host
 */
class CmdAddGame : public AbstractCommand {
public:
    CmdAddGame(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        if (args.size() != 2) {
            std::cerr << "Error: must set number of players and game type" << std::endl;
            return;
        }

        Game game(args[0], args[1], client_id, &controller);
        controller.add_game(client_id, game);
        controller.update_player_status(client_id, client_id);

        controller.send_call("joined"+std::to_string(args[0])+"1"+std::to_string(args[1]), client_id);
    }

private:
    ServerController& controller;
};

#endif // __CMDADDGAME__
