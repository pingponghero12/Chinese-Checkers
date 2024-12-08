#ifndef __HANDLECLIENTS__
#define __HANDLECLIENTS__

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

extern std::vector<int> client_sockets;
extern std::mutex client_mutex;

void broadcast_message(const std::string& message);

// Każdy klient jest obsługiwany w osobnym wątku.
void handle_client(int client_socket, int client_number, struct sockaddr_in client_addr);

#endif // __HANDLECLIENTS__
