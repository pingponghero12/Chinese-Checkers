#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> // For inet_pton
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Funkcja obsługująca odbieranie wiadomości od serwera.
// Działa w osobnym wątku, aby umożliwić jednoczesne wysyłanie i odbieranie wiadomości.
void receiveMessages(int socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE); // Wyczyszczenie bufora przed odbiorem danych.
        int bytes_read = read(socket, buffer, BUFFER_SIZE); // Odczyt danych z gniazda.
        if (bytes_read <= 0) { // Sprawdzenie, czy połączenie zostało zerwane.
            std::cout << "Disconnected from server.\n";
            close(socket); // Zamknięcie gniazda połączenia.
            exit(0); // Zakończenie wątku w przypadku rozłączenia.
        }
        std::cout << buffer; // Wyświetlenie odebranej wiadomości.
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Utworzenie gniazda klienta.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error"); // Wyświetlenie błędu w przypadku niepowodzenia tworzenia gniazda.
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr)); // Wyzerowanie struktury adresu serwera.
    serv_addr.sin_family = AF_INET; // Użycie protokołu IPv4.
    serv_addr.sin_port = htons(PORT); // Ustawienie numeru portu w formacie sieciowym.

    // Konwersja adresu IP z formatu tekstowego na binarny.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
        std::cerr << "Invalid address/ Address not supported \n"; // Wyświetlenie błędu w przypadku nieprawidłowego adresu IP.
        return -1;
    }

    // Próba nawiązania połączenia z serwerem.
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed"); // Wyświetlenie błędu w przypadku niepowodzenia połączenia.
        return -1;
    }

    std::cout << "Connected to server.\n";

    // Uruchomienie wątku odbierającego wiadomości od serwera.
    std::thread receiver(receiveMessages, sock);
    receiver.detach(); // Wątek działa niezależnie od głównego wątku.

    // Główny wątek obsługuje wysyłanie wiadomości do serwera.
    std::string message;
    while (true) {
        std::getline(std::cin, message); // Odczyt wiadomości z wejścia użytkownika.
        if (message.empty()) continue; // Pominięcie pustych wiadomości.
        message += "\n"; // Dodanie nowej linii dla czytelności.
        send(sock, message.c_str(), message.length(), 0); // Wysłanie wiadomości do serwera.
    }

    close(sock); // Zamknięcie gniazda po zakończeniu programu.
    return 0;
}
