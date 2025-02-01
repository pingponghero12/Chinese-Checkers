#include "server.hpp"
#include "server_controller.hpp"

#include <thread>

/**
 * @brief Construct a new Server object
 * 
 * Initializes a Server instance with the specified port number and creates
 * a new ServerController associated with this server. The server file descriptor
 * is initialized to zero.
 *
 * @param port The port number to which the server will listen for incoming connections.
 */
Server::Server(int port) : port(port), server_fd(0) {
    controller = new ServerController(this);
}

/**
 * @brief Destroy the Server object
 * 
 * Closes the server file descriptor when the Server instance is destroyed.
 */
Server::~Server() {
    close(server_fd);
}

/**
 * @brief Initialize the server
 * 
 * This function initializes the server by creating a socket, setting socket options, 
 * binding the socket to an address and port, and starting to listen for incoming client 
 * connections. It returns true if successful, or false in case of errors.
 * 
 * @return True if server initialization was successful, false otherwise.
 */
bool Server::init_server() {
    struct sockaddr_in address;

    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number. Port must be between 1 and 65535" << std::endl;
        return false;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return false;;
    }

    // Setup of socket options, to forbid using address and port again
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        return false;;
    }

    address.sin_family = AF_INET;
    // Accept connections on every interface
    address.sin_addr.s_addr = INADDR_ANY;
    // Port number to network format
    address.sin_port = htons(port);

    // Binding of socket with address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return false;
    }

    // Start listening
    if (listen(server_fd, 10) < 0) { // Queue length 10
        perror("Listen failed");
        close(server_fd);
        return false;;
    }

    std::cout << "Server listening on port " << port << "\n";
    return true;
}

/**
 * @brief Start the server
 * 
 * This function starts the server by launching a separate thread for handling 
 * console input while entering a loop to accept new clients.
 */
void Server::start_server() {
    // Other thread for console I/O
    std::thread input_thread(&Server::server_input_thread, this);
    input_thread.detach();

    loop_new_clients();
}

/**
 * @brief Handle console input in a separate thread
 * 
 * This thread continuously reads input from the console and broadcasts
 * any non-empty messages received to all connected clients.
 */
void Server::server_input_thread() {
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message.empty()) continue;
        message += "\n";

        broadcast_message("Server: " + message);
    }
}

/**
 * @brief Loop to accept new client connections
 * 
 * This method continuously listens for and accepts new client connections.
 * For each new connection, it creates a thread to handle the client and
 * updates the client's status in the server controller.
 */
void Server::loop_new_clients() {
    int client_id = 0;

    while (true) {
        int new_socket;
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addrlen);

        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "New client connected: Client[" << client_id << "], IP: " 
                  << inet_ntoa(client_addr.sin_addr) << "\n";

        controller->update_player_status(client_id, -1);

        // Lock for thread safety
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            client_sockets[client_id] = new_socket;
        }

        // New client thread
        std::thread client_thread(&Server::handle_client, this, new_socket, client_id, client_addr);
        client_thread.detach();

        client_id++;
    }
}

/**
 * @brief Broadcast a message to all connected clients
 * 
 * This function sends a specified message to all currently connected clients
 * by iterating through the client_sockets map. It ensures thread safety
 * by using a mutex lock.
 *
 * @param message The message to be broadcast to all clients.
 */
void Server::broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(client_mutex);

    for (const auto& pair : client_sockets) {
        send(pair.second, message.c_str(), message.length(), 0);
    }
}

/**
 * @brief Send a message to a specific client
 * 
 * Sends a message to a specific client identified by client_id. It checks 
 * for the validity of the client ID and handles any send errors.
 *
 * @param message The message to be sent to the client.
 * @param client_id The ID of the client who will receive the message.
 */
void Server::send_message(const std::string& message, const int& client_id) {
    std::lock_guard<std::mutex> lock(client_mutex);

    auto it = client_sockets.find(client_id);
    if (it == client_sockets.end()) {
        std::cerr << "error: invalid client id: " << client_id << std::endl;
        return;
    }

    if (send(client_sockets[client_id], message.c_str(), message.length(), 0) < 0) {
        perror("error: send() failed");
    }
}

/**
 * @brief Handle communication with a connected client
 * 
 * This function manages the reading of messages from a client socket. It 
 * processes incoming messages and invokes command parsing via the server controller.
 * It also handles client disconnection and cleans up the resources.
 * 
 * @param client_socket The socket descriptor for the connected client.
 * @param client_id The ID of the client.
 * @param client_addr The address information of the connected client.
 */
void Server::handle_client(int client_socket, int client_id, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    while (true) {
        // Clean buffer
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);

        // Check for error or disconnect
        if (bytes_read <= 0) {
            std::cout << "Client[" << client_id << "] disconnected. IP: " 
                      << inet_ntoa(client_addr.sin_addr) << "\n";

            close(client_socket);
            break;
        }

        // Received buffer to string
        std::string message = std::string(buffer);
        std::cout << "Client[" << client_id << "]: " << message;
        controller->parse_call(message, client_id);
    }

    // Deletion of disconnected client from list of active clients
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        client_sockets.erase(client_id);
    }
}
