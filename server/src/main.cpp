#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.hpp"

#define PORT 8080

int main() {
    Server server(PORT);

    if (!server.init_server()) {
        return EXIT_FAILURE;
    }

    server.start_server();
    return 0;
}
