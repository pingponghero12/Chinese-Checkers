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

#include "handle_clients.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

extern std::vector<int> client_sockets;
extern std::mutex client_mutex;

// Wątek obsługujący wejście serwera (komendy administratora) z konsoli.
void server_input_thread();

#endif // __SERVER__
