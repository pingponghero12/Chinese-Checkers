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

class CmdListHistory: public AbstractCommand {
public:
    CmdListHistory(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        // std::vector<Game> games = controller.game_list();
        std::vector<std::vector<int>> games = controller.dbconn->get_games();

        /*
        std::string out = "lobbies:";
        for (size_t i=0; i < games.size(); i++) {
            out = out + std::to_string(std::to_string(games[i][0]) + " - " + std::to_string(games[i].get_players()) + "/" + std::to_string(games[i].get_game_type()) + ",";
        }
        controller.send_call(out, client_id);
        */
    }

private:
    ServerController& controller;
};

#endif // __CMDLISTHISTORY__
