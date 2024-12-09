#ifndef __SERVERCONTROLLER__
#define __SERVERCONTROLLER__

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

class ServerController {
public:
    ServerController();
    ~ServerController();

    void handle_input(char* buffer);

private:
    std::vector<Game> games;

    void create_game(const int& user, const int& players);
    void list_games(const int& user);
    void join_game(const int& user, const int& game_id);

    void send_message(const std::string& message, const int& client_id);

};

#endif // __SERVERCONTROLLER__
