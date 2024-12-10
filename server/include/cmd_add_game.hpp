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

class CmdAddGame : public AbstractCommand {
public:
    CmdAddGame(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_number) {
        Game* game = new Game();
        controller.add_game(*game);

        controller.send_call("Added game\n", client_number);
    }

private:
    ServerController& controller;
};

#endif // __CMDADDGAME__
