#ifndef __CMDMOVE__
#define __CMDMOVE__

#include <iostream>
#include <vector>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_map>

#include "abstract_command.hpp"
#include "server_controller.hpp"
#include "game.hpp"

class CmdMove : public AbstractCommand {
public:
    CmdMove(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        if (args.size() != 4) {
            std::cerr << "Error: <x1> <y1> <x2> <y2>" << std::endl;
            return;
        }

        int game_id = controller.get_player_status(client_id);
        int x1 = args[0];
        int y1 = args[1];
        int x2 = args[2];
        int y2 = args[3];

        {
            std::lock_guard<std::mutex> lock(controller.status_mutex);
            controller.current_games[game_id].move(client_id, x1, y1, x2, y2);
        }
    }

private:
    ServerController& controller;
};

#endif // __CMDMOVE__
