#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "client.hpp"

/// Maximum size for the receive buffer
#define BUFFER_SIZE 1024

/**
 * @brief Constructs a new Client object
 * @param ip The IP address of the server to connect to
 * @param port The port number to connect to
 */
Client::Client(const std::string& ip, int port)
    : server_ip(ip), port(port), sock(0), connected(false) {}

/**
 * @brief Destructor ensures proper cleanup by disconnecting if necessary
 */
Client::~Client() {
    disconnect();
}

/**
 * @brief Message receiving thread function
 * @details Continuously receives messages from the server while connected.
 *          When a message is received, it's passed to the message callback
 *          if one is registered.
 */
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
        if (message_callback) {
            message_callback(std::string(buffer));
        }
    }
}

/**
 * @brief Transforms client messages into server protocol format
 * @param input The input message to transform
 * @return std::string The transformed message
 * @details Converts text commands to numeric codes:
 *          - "create" -> "1"
 *          - "list" -> "2"
 *          - "join" -> "3"
 *          - "move" -> "4"
 *          - "exit" -> "5"
 */
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
    } else if (command == "exit") {
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

/**
 * @brief Establishes connection with the server
 * @return bool True if connection successful, false otherwise
 * @details Creates a TCP socket and attempts to connect to the server
 *          using the stored IP address and port number.
 */
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

/**
 * @brief Starts the message receiving thread
 * @details Creates and detaches a thread that continuously listens
 *          for incoming messages from the server
 */
void Client::start_receiving() {
    receiver_thread = std::thread(&Client::receive_messages, this);
    receiver_thread.detach();
}

/**
 * @brief Sends a message to the server
 * @param message The message to send
 * @details Transforms the message according to the server protocol
 *          and sends it over the socket connection
 */
void Client::send_message(const std::string& message) {
    // Change string command into integer
    std::string transformedMessage = transform_message(message);
    transformedMessage += "\n";
    send(sock, transformedMessage.c_str(), transformedMessage.length(), 0);
}

/**
 * @brief Sets the callback function for received messages
 * @param callback Function to be called when a message is received
 */
void Client::set_message_callback(MessageCallback callback) {
    message_callback = callback;
}

/**
 * @brief Disconnects from the server
 * @details Closes the socket and sets the connected flag to false
 */
void Client::disconnect() {
    if (connected) {
        close(sock);
        connected = false;
    }
}