#ifndef __CMDJOINGAME__
#define __CMDJOINGAME__

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

class CmdJoinGame : public AbstractCommand {
public:
    CmdJoinGame(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_number) {
        if (args.size() != 1) {
            std::cerr << "Error: must set number of players" << std::endl;
            return;
        }

        int game_id = args[0];

        controller.update_player_status(client_number, game_id);

        controller.current_games[game_id].add_player(client_number);

        controller.send_call("Joined game\n", client_number);
    }

private:
    ServerController& controller;
};

#endif // __CMDJOINGAME__
