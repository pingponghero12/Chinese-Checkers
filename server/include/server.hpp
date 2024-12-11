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
#include <unordered_map>

#include "server_controller.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

class Server {
public:
    Server(int port = PORT);
    ~Server();

    bool init_server();
    void start_server();

    void send_message(const std::string& message, const int& client_id);
    void broadcast_message(const std::string& message);
    void handle_client(int client_socket, int client_id, struct sockaddr_in client_addr);

private:
    int port;
    int server_fd;
    std::unordered_map<int, int> client_sockets;
    std::mutex client_mutex;
    ServerController* controller;

    void server_input_thread();
    void loop_new_clients();
};

#endif // __SERVER__
