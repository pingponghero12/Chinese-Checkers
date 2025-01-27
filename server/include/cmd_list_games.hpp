#ifndef __CMDLISTGAMES__
#define __CMDLISTGAMES__

#include <vector>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "abstract_command.hpp"
#include "server_controller.hpp"


/**
 * @brief Command to list games
 * The command lists all games on the server
 * The command sends a message to the client with the list of games
 * Each game is represented by an id and the number of players in the game
 */

class CmdListGames : public AbstractCommand {
public:
    CmdListGames(ServerController& controller) : controller(controller) {}

    void execute(const std::vector<int>& args, int client_id) {
        std::vector<Game> games = controller.game_list();

        std::string out = "lobbies:";
        for (size_t i=0; i < games.size(); i++) {
            out = out + std::to_string(games[i].get_id()) + " - " + std::to_string(games[i].get_players()) + "/" + std::to_string(games[i].get_game_type()) + ",";
        }
        controller.send_call(out, client_id);
    }

private:
    ServerController& controller;
};

#endif // __CMDLISTGAMES__
