#ifndef __SERVER__
#define __SERVER__

#include <string>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_map>

#include "server_controller.hpp"

#define BUFFER_SIZE 1024

class Server {
public:
    Server(int port);
    ~Server();

    virtual bool init_server();
    virtual void start_server();

    virtual void send_message(const std::string& message, const int& client_id);
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
