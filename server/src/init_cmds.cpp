#include "server_controller.hpp"

#include "cmd_add_game.hpp"
#include "cmd_list_games.hpp"
#include "cmd_join_game.hpp"
#include "cmd_exit_game.hpp"
#include "cmd_move.hpp"

void ServerController::initialize_commands() {
    command_registry_[1] = std::make_unique<CmdAddGame>(*this);
    command_registry_[2] = std::make_unique<CmdListGames>(*this);
    command_registry_[3] = std::make_unique<CmdJoinGame>(*this);
    command_registry_[4] = std::make_unique<CmdMove>(*this);
    command_registry_[5] = std::make_unique<CmdExitGame>(*this);
}
