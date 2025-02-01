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
        if (args.size() != 2) {
            std::cerr << "Error: <game id> <move id>" << std::endl;
            return;
        }
        int game_id = args[0] + 1;
        int move_id = args[1];

        std::vector<int> moves = controller.dbconn->get_move(game_id, move_id);

        if (moves.empty()) {
            std::cout << "moves empty" << std::endl;
            controller.send_call("exited", client_id);
        }
        else {
            std::cout << "moves sending normal" << std::endl;
            std::string out =  "move " + std::to_string(moves[0]) + " " + std::to_string(moves[1]) + " " + std::to_string(moves[2]) + " " + std::to_string(moves[3]);

            controller.send_call(out, client_id);
        }
    }

private:
    ServerController& controller;
};

#endif // __CMDMOVEHISTORY__
