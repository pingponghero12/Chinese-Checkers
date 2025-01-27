#include "server_controller.hpp"

#include "cmd_add_game.hpp"
#include "cmd_list_games.hpp"
#include "cmd_join_game.hpp"
#include "cmd_exit_game.hpp"
#include "cmd_move.hpp"
#include "cmd_list_history.hpp"
#include "cmd_move_history.hpp"

// I tried to make a Factory out of this, wasted like 2 hours, there are like 300 lines of error. And that all just to achive same result, since here I only need to modify one file, which is different from `server_controller.cpp`
/**
 * @brief Construct a new Server Controller:: Server Controller object
 * Maps the commands to the command registry
 */
void ServerController::initialize_commands() {
    command_registry_[1] = std::make_unique<CmdAddGame>(*this);
    command_registry_[2] = std::make_unique<CmdListGames>(*this);
    command_registry_[3] = std::make_unique<CmdJoinGame>(*this);
    command_registry_[4] = std::make_unique<CmdMove>(*this);
    command_registry_[5] = std::make_unique<CmdExitGame>(*this);
    command_registry_[6] = std::make_unique<CmdListHistory>(*this);
    command_registry_[7] = std::make_unique<CmdMoveHistory>(*this);
}
