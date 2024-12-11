#ifndef __CMDLISTGAMES__
#define __CMDLISTGAMES__

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

class CmdListGames : public AbstractCommand {
public:
    CmdListGames(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        std::vector<Game> games = controller.game_list();

        std::string out = "";
        for (size_t i=0; i < games.size(); i++) {
            out = out + std::to_string(games[i].get_id()) + " - " + std::to_string(games[i].get_players()) + "/" + std::to_string(games[i].get_game_type()) + "\n";
        }
        controller.send_call(out, client_id);
    }

private:
    ServerController& controller;
};

#endif // __CMDLISTGAMES__
