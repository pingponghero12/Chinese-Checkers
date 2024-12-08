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

std::vector<int> client_sockets;
std::mutex client_mutex;


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Utworzenie gniazda serwera.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed"); // Wyświetlenie błędu w przypadku niepowodzenia tworzenia gniazda.
        exit(EXIT_FAILURE);
    }

    // Ustawienie opcji gniazda, aby umożliwić ponowne użycie adresu i portu.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt"); // Wyświetlenie błędu w przypadku niepowodzenia ustawienia opcji.
        close(server_fd); // Zamknięcie gniazda przed wyjściem.
        exit(EXIT_FAILURE);
    }

    // Konfiguracja struktury adresu serwera.
    address.sin_family = AF_INET; // Użycie protokołu IPv4.
    address.sin_addr.s_addr = INADDR_ANY; // Akceptowanie połączeń na wszystkich dostępnych interfejsach.
    address.sin_port = htons(PORT); // Konwersja numeru portu na format sieciowy.

    // Powiązanie gniazda serwera z określonym adresem i portem.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Rozpoczęcie nasłuchiwania na połączenia od klientów.
    if (listen(server_fd, 10) < 0) { // Kolejka połączeń o długości 10.
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "\n";

    // Rozpoczęcie osobnego wątku do obsługi wejścia z konsoli.
    std::thread input_thread(server_input_thread);
    input_thread.detach();

     // Numeracja klientów (rozpoczyna się od 1).
    int client_number = 1;

    // Główna pętla do obsługi nowych połączeń klientów.
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addrlen); // Akceptacja nowego połączenia.

        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Wyświetlenie informacji o nowym kliencie.
        std::cout << "New client connected: Client[" << client_number << "], IP: " 
                  << inet_ntoa(client_addr.sin_addr) << "\n";

        // Dodanie gniazda nowego klienta do listy aktywnych klientów.
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            client_sockets.push_back(new_socket);
        }

        // Uruchomienie nowego wątku do obsługi komunikacji z klientem.
        std::thread client_thread(handle_client, new_socket, client_number, client_addr);
        client_thread.detach();

        client_number++; // Zwiększenie licznika klientów dla kolejnego klienta.
    }

    close(server_fd);

    return 0;
}
