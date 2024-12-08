#include "server.hpp"

Server::Server(int port) : port(port), server_fd(0) {}

Server::~Server() {
    close(server_fd);
}

bool Server::init_server() {
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return false;;
    }

    // Setup of socket options, to fobid using address and port again
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        return false;;
    }

    address.sin_family = AF_INET;
    // Accept connections on every interface
    address.sin_addr.s_addr = INADDR_ANY;
    // Portal number to network format
    address.sin_port = htons(PORT);

    // Binding of socket with address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return false;
    }

    // Start listening
    if (listen(server_fd, 10) < 0) { // Queue lenght 10
        perror("Listen failed");
        close(server_fd);
        return false;;
    }

    std::cout << "Server listening on port " << PORT << "\n";
    return true;
}

void Server::start_server() {
    // Other thread for console I/O
    std::thread input_thread(&Server::server_input_thread, this);
    input_thread.detach();

    loop_new_clients();
}

void Server::server_input_thread() {
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message.empty()) continue;
        message += "\n";

        broadcast_message("Server: " + message);
    }
}

void Server::loop_new_clients() {
    int client_number = 1;

    while (true) {
        int new_socket;
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addrlen);

        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "New client connected: Client[" << client_number << "], IP: " 
                  << inet_ntoa(client_addr.sin_addr) << "\n";

        // Personally I'm more of lock() unlock() guy
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            client_sockets.push_back(new_socket);
        }

        // New client thread
        std::thread client_thread(&Server::handle_client, this, new_socket, client_number, client_addr);
        client_thread.detach();

        client_number++;
    }
}

void Server::broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(client_mutex);

    for (const int& socket : client_sockets) {
        send(socket, message.c_str(), message.length(), 0);
    }
}

void Server::send_message(const std::string& message, const int& client_id) {
    std::lock_guard<std::mutex> lock(client_mutex);

    if (client_id < 0 || client_id >= client_sockets.size()) {
        std::cerr << "error: invalid client id: " << client_id << std::endl;
        return;
    }

    if (send(socket, message.c_str(), message.length(), 0) < 0) {
        perror("error: send() failed);
    }
}

void Server::handle_client(int client_socket, int client_number, struct sockaddr_in client_addr) {
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

        // Received buffor to string
        std::string message = std::string(buffer);
        std::cout << "Client[" << client_number << "]: " << message;
    }

    // Deletion of disconnected client from list of active
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        client_sockets.erase(
            std::remove(client_sockets.begin(), client_sockets.end(), client_socket),
            client_sockets.end()
        );
    }
}
