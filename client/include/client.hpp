#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <thread>
#include <functional>
#include <vector>

class Client {
public:
    using MessageCallback = std::function<void(const std::string&)>;

    Client(const std::string& server_ip, int port);
    ~Client();

    bool connect_to_server();
    void start_receiving();
    void send_message(const std::string& message);
    void disconnect();
    void set_message_callback(MessageCallback callback);
    std::vector<std::pair<int, int>> possible_moves(int x, int y);
    std::vector<std::vector<int>> board_state();

private:
    void receive_messages();
    std::string transform_message(const std::string& message);

    std::string server_ip;
    int port;
    int sock;
    std::thread receiver_thread;
    bool connected;
    MessageCallback message_callback;
};

#endif // CLIENT_H
