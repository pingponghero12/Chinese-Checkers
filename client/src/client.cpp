#include <iostream>
#include <thread>
#include <string>
#include <sstream> 
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <memory>

#include "client.hpp"

#define BUFFER_SIZE 1024

Client::Client(const std::string& ip, int port)
    : server_ip(ip), port(port), sock(0), connected(false), board(nullptr) {}

Client::~Client() {
    disconnect();
}

std::vector<int> parse_move(const std::string& message) {
    std::vector<int> coords;
    std::istringstream iss(message);
    std::string command;
    int value;

    iss >> command;
    
    while (iss >> value) {
        coords.push_back(value);
    }

    return coords;
}

void Client::receive_messages() {
    char buffer[BUFFER_SIZE];
    std::cout << "started reveive_messages\n";
    while (connected) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(sock, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            std::cout << "Disconnected from server.\n";
            connected = false;
            close(sock);
            break;
        }

        std::string message = std::string(buffer);
        if (message.substr(0, 6) == "joined") {
            create_board(message[6] - '0');
        } 
        else if (message.substr(0, 6) == "exited") {
            exit_board();
        }
        else if (message.substr(0, 4) == "move") {
            std::vector<int> mv = parse_move(message);
            board->move(mv[0], mv[1], mv[2], mv[3]);
        }

        if (message_callback) {
            message_callback(std::string(buffer));
        }
    }
}

std::vector<std::pair<int, int>> Client::possible_moves(int x, int y) {
    return board->possible_moves(x, y);
}

std::vector<std::vector<int>> Client::board_state() {
    std::vector<std::vector<char>> char_board = board->getFields();
    std::vector<std::vector<int>> result(17, std::vector<int>(25));

    for (size_t i = 0; i < char_board.size(); i++) {
        for (size_t j = 0; j < char_board[i].size(); j++) {
            char c = char_board[i][j];
            if (c == '0' || c == ' ') {
                result[i][j] = -1;
            } else {
                // Convert '1' to 0, '2' to 1, etc.
                result[i][j] = (c - '1');
            }
        }
    }

    return result;
}

void Client::create_board(int players) {
    board = std::unique_ptr<Standard_Board>(new Standard_Board(players));
}

void Client::exit_board() {
    board = nullptr;
}


std::string Client::transform_message(const std::string& input) {
    std::istringstream iss(input);
    std::ostringstream oss;

    std::string command;
    iss >> command; 

    
    if (command == "create") {
        oss << "1";
    } else if (command == "list") {
        oss << "2";
    } else if (command == "join") {
        oss << "3";
    } else if (command == "move") {
        oss << "4";
    }else if (command == "exit"){
        oss << "5";
    } else {
        // If unrecognized leave as is
        oss << command;
    }

    // Add the rest of arguments
    std::string rest;
    std::getline(iss, rest); 
    oss << rest;

    return oss.str();
}

bool Client::connect_to_server() {
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return false;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported \n";
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return false;
    }

    connected = true;
    std::cout << "Connected true" << std::endl;
    return true;
}

void Client::start_receiving() {
    receiver_thread = std::thread(&Client::receive_messages, this);
    receiver_thread.detach();
}

void Client::send_message(const std::string& message) {
    // Change string command into integer
    std::string transformedMessage = transform_message(message);

    transformedMessage += "\n"; 
    send(sock, transformedMessage.c_str(), transformedMessage.length(), 0);
}

void Client::set_message_callback(MessageCallback callback) {
    message_callback = callback;
}

void Client::disconnect() {
    if (connected) {
        close(sock);
        connected = false;
    }
}
