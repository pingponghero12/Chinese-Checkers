#include "server.hpp"

void server_input_thread() {
    std::string message;
    while (true) {
        std::getline(std::cin, message); // Odczyt wiadomości z konsoli.
        if (message.empty()) continue; // Pominięcie pustych wiadomości.
        message += "\n"; // Dodanie nowej linii dla poprawy czytelności.
        broadcast_message("Server: " + message); // Wysłanie wiadomości do wszystkich klientów.
    }
}
