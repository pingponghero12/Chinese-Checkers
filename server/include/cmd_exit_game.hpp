#ifndef __CMDEXITGAME__
#define __CMDEXITGAME__

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

class CmdExitGame : public AbstractCommand {
public:
    CmdExitGame(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_number) {
        if (args.size() != 0) {
            std::cerr << "Error: function takes no args" << std::endl;
            return;
        }

        int game_id = controller.get_player_status(client_number);
        controller.update_player_status(client_number, -1);

        {
            std::lock_guard<std::mutex> lock(controller.games_mutex);
            auto it = controller.current_games.find(game_id);
            if (it != controller.current_games.end()) {
                it->second.remove_player(client_number);
            } else {
                std::cerr << "Error: Game not found" << std::endl;
                controller.send_call("Error: Game not found\n", client_number);
                return;
            }
        }

        controller.send_call("Exited game\n", client_number);
    }

private:
    ServerController& controller;
};

#endif // __CMDEXITGAME__
