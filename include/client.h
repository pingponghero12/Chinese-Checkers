// client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <thread>
#include <functional>

class Client {
public:
    // Type definition for the callback function
    using MessageCallback = std::function<void(const std::string&)>;

    Client(const std::string& server_ip, int port);
    ~Client();

    bool connect_to_server();
    void start_receiving();
    void send_message(const std::string& message);
    void disconnect();

    // Method to set the callback
    void set_message_callback(MessageCallback callback);

private:
    void receive_messages();
    std::string transformMessage(const std::string& input);

    std::string server_ip;
    int port;
    int sock;
    std::thread receiver_thread;
    bool connected;
    MessageCallback message_callback;
};

#endif // CLIENT_H
