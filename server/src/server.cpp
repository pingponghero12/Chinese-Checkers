#include "server.hpp"

bool init_server(int& server_fd) {
    struct sockaddr_in address;
    // socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return false;;
    }

    // Ustawienie opcji gniazda, aby umożliwić ponowne użycie adresu i portu.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        return false;;
    }

    // Konfiguracja struktury adresu serwera.
    address.sin_family = AF_INET;
    // Akceptowanie połączeń na wszystkich dostępnych interfejsach.
    address.sin_addr.s_addr = INADDR_ANY;
    // Konwersja numeru portu na format sieciowy.
    address.sin_port = htons(PORT);

    // Powiązanie gniazda serwera z określonym adresem i portem.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return false;
    }

    // Rozpoczęcie nasłuchiwania na połączenia od klientów.
    if (listen(server_fd, 10) < 0) { // Kolejka połączeń o długości 10.
        perror("Listen failed");
        close(server_fd);
        return false;;
    }

    std::cout << "Server listening on port " << PORT << "\n";

    // Rozpoczęcie osobnego wątku do obsługi wejścia z konsoli.
    std::thread input_thread(server_input_thread);
    input_thread.detach();

    return true;
}

void server_input_thread() {
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message.empty()) continue;
        message += "\n";

        broadcast_message("Server: " + message);
    }
}

void loop_new_clients(const int& server_fd) {
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

        // Dodanie gniazda nowego klienta do listy aktywnych klientów.
        // Personally I'm more of lock() unlock() guy
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            client_sockets.push_back(new_socket);
        }

        // Uruchomienie nowego wątku do obsługi komunikacji z klientem.
        std::thread client_thread(handle_client, new_socket, client_number, client_addr);
        client_thread.detach();

        client_number++;
    }
}
