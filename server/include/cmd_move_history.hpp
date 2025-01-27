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
#include "db_connector.hpp"

class CmdMoveHistory: public AbstractCommand {
public:
    CmdMoveHistory(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        // std::vector<int> get_move(int game, int move_id);
        if (args.size() != 1) {
            std::cerr << "Error: <game id> <move id>" << std::endl;
            return;
        }

        std::vector<int> moves = controller.dbconn->get_move(args[0], args[1]);

        std::string out =  "move," + std::to_string(moves[0]) + "," + std::to_string(moves[1]) + "," + std::to_string(moves[2]) + "," + std::to_string(moves[2]);

        controller.send_call(out, client_id);
    }

private:
    ServerController& controller;
};

#endif // __CMDMOVEHISTORY__
