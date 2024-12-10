#include "server_controller.hpp"
#include "server.hpp"

ServerController::ServerController(Server* server) : server(server) {
    server = server;
    initialize_commands();
}

void ServerController::initialize_commands() {
    // command_registry_[1] = std::make_unique
}

std::vector<int> parse_stvi(const std::string& message) {
    std::istringstream iss(message);
    std::vector<int> args;
    int value;
    while (iss >> value) {
        args.push_back(value);
    }
    return args;
}

void ServerController::parse_call(const std::string& message, int client_number) {
    std::vector<int> args = parse_stvi(message);

    if (args.empty()) {
        return;
    }


    int command_index = args[0];
    args.erase(args.begin());

    std::cout << "execute " <<args[0]<< std::endl;
    send_call("Skibidi welcomes you", client_number);
    auto it = command_registry_.find(command_index);
    if (it != command_registry_.end()) {
        //it->second->execute(args, client_number);
    }
    else {
        std::cerr << "Command not found: " << command_index << std::endl;
    }
}

void ServerController::send_call(const std::string& message, int client_number) {
    server->send_message(message, client_number);
}
