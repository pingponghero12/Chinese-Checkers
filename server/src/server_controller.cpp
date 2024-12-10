#include "server_controller.hpp"

ServerController::ServerController() {
    initialize_commands();
}

ServerController::initialize_conmmands() {
    // command_registry_[1] = std::make_unique
}

std::vector<int> parse_message(const std::string& message) {
    std::istringstream iss(message);
    std::vector<int> args;
    int value;
    while (iss >> value) {
        args.push_back(value);
    }
    return args;
}

ServerController::parse_call(const std::string& message, int client_number) {
    std::vector<int> args = parse_message(message);

    if (args.empty()) {
        return;
    }


    int command_index = args[0];
    args.erase(args.begin());

    auto it = command_registry_.find(command_index);
    if (it != command_registry_.end()) {
        it->second->execute(args, client_number);
    }
    else {
        std::cerr << "Command not found: " << command_index << std::endl;
    }
}

