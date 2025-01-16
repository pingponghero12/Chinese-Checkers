#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.hpp"

#define PORT 8080
/**
 * @brief Construct a new Server:: Server object
 * Creates a new server with a given port
 * All the games are played on a one server
 */

int main() {
    Server server(PORT);

    if (!server.init_server()) {
        return EXIT_FAILURE;
    }

    server.start_server();
    return 0;
}
