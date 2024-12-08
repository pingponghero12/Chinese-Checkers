#ifndef __SERVER__
#define __SERVER__

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

#define PORT 8080
#define BUFFER_SIZE 1024

class Server {
public:
    Server(int port = PORT);
    ~Server();

    bool init_server();
    void start_server();

private:
    int port;
    int server_fd;
    std::vector<int> client_sockets;
    std::mutex client_mutex;

    void server_input_thread();
    void loop_new_clients();
    void broadcast_message(const std::string& message);

    // Każdy klient jest obsługiwany w osobnym wątku.
    void handle_client(int client_socket, int client_number, struct sockaddr_in client_addr);
};

#endif // __SERVER__
