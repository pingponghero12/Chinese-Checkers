#ifndef __CMDLISTHISTORY__
#define __CMDLISTHISTORY__

#include <vector>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "abstract_command.hpp"
#include "server_controller.hpp"
#include "db_connector.hpp"

class CmdListHistory: public AbstractCommand {
public:
    CmdListHistory(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        if (args.size() != 0) {
            std::cerr << "Error: args should be empty" << std::endl;
            return;
        }

        std::vector<std::string> games = controller.dbconn->get_games();
        std::string out = "history:\n";

        for (size_t i=0; i < games.size(); i++) {
            out = out + games[i] + "\n";
        }

        controller.send_call(out, client_id);
    }

private:
    ServerController& controller;
};

#endif // __CMDLISTHISTORY__
