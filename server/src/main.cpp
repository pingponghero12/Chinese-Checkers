#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.hpp"
#include "handle_clients.hpp"

std::vector<int> client_sockets;
std::mutex client_mutex;

bool server_on = true;
std::mutex server_on_mutex;

int main() {
    int server_fd;

    if (!init_server(server_fd)) {
        return EXIT_FAILURE;
    }

    loop_new_clients(server_fd);

    return 0;
}
