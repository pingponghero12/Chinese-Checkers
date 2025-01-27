#ifndef __CMDMOVEHISTORY__
#define __CMDMOVEHISTORY__

#include <vector>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "abstract_command.hpp"
#include "server_controller.hpp"

class CmdMoveHistory: public AbstractCommand {
public:
    CmdMoveHistory(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        // Pass
    }

private:
    ServerController& controller;
};

#endif // __CMDMOVEHISTORY__
