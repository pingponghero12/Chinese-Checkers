#include "handle_clients.hpp"

void broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(client_mutex);

    for (const int& socket : client_sockets) {
        send(socket, message.c_str(), message.length(), 0);
    }
}

void handle_client(int client_socket, int client_number, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    while (true) {
        // Clean buffer
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);

        // Check for error or disconnect
        if (bytes_read <= 0) {
            std::cout << "Client[" << client_number << "] disconnected. IP: " 
                      << inet_ntoa(client_addr.sin_addr) << "\n";

            close(client_socket);
            break;
        }

        // Przekształcenie otrzymanego bufora na string w celu łatwiejszej obsługi.
        std::string message = std::string(buffer);
        std::cout << "Client[" << client_number << "]: " << message;
    }

    // Usunięcie gniazda rozłączonego klienta z listy aktywnych klientów.
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        client_sockets.erase(
            std::remove(client_sockets.begin(), client_sockets.end(), client_socket),
            client_sockets.end()
        );
    }
}
