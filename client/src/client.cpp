#include <iostream>
#include <thread>
#include <string>
#include <sstream> // Do obsługi istringstream i ostringstream
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For inet_pton
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void receiveMessages(int socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            std::cout << "Disconnected from server.\n";
            close(socket);
            exit(0);
        }
        std::cout << buffer;
    }
}

std::string transformMessage(const std::string& input) {
    std::istringstream iss(input);
    std::ostringstream oss;

    std::string command;
    iss >> command; // Pobiera pierwszy wyraz wiadomości

    // Zamiana komend tekstowych na liczby
    if (command == "create") {
        oss << "1";
    } else if (command == "list") {
        oss << "2";
    } else if (command == "join") {
        oss << "3";
    } else if (command == "move") {
        oss << "4";
    } else {
        // Jeśli nie rozpoznano komendy, pozostaw ją niezmienioną
        oss << command;
    }

    // Dodaj resztę argumentów
    std::string rest;
    std::getline(iss, rest); // Pobiera pozostałą część wiadomości
    oss << rest;

    return oss.str();
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    std::cout << "Connected to server.\n";
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Create game (type create <number of players>)" << std::endl;
    std::cout << "2. List games" << std::endl;
    std::cout << "3. Join game (type join <game id>)" << std::endl;
    std::cout << "4. Move" << std::endl;

    std::thread receiver(receiveMessages, sock);
    receiver.detach();

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message.empty()) continue;

        // Zamień tekstową komendę na numeryczną
        std::string transformedMessage = transformMessage(message);

        transformedMessage += "\n"; // Dodanie nowej linii
        send(sock, transformedMessage.c_str(), transformedMessage.length(), 0);
    }

    close(sock);
    return 0;
}
