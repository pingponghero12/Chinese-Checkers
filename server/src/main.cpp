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

int main() {
    Server server;

    if (!server.init_server()) {
        return EXIT_FAILURE;
    }

    server.start_server();
    return 0;
}
