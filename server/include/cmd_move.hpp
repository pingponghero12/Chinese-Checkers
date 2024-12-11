#ifndef __CMDMOVE__
#define __CMDMOVE__

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

class CmdMove : public AbstractCommand {
public:
    CmdMove(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        if (args.size() != 2) {
            std::cerr << "Error: <x> <y>" << std::endl;
            return;
        }

        int game_id = controller.get_player_status(client_id);
        int x = args[0];
        int y = args[1];

        {
            std::lock_guard<std::mutex> lock(controller.status_mutex);
            controller.current_games[game_id].move(client_id, x, y);
        }
    }

private:
    ServerController& controller;
};

#endif // __CMDMOVE__
