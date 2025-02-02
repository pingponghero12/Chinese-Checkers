#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>

class Board;

/**
 * @brief The Client class is responsible for connecting to the server, sending and receiving messages.
 * It also provides a method to get the possible moves for a given position and the current board state.
 * The class uses a callback to notify the user when a message is received.
 * The class is not responsible for the game logic, it only provides the necessary information to the user.
 */

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
    std::vector<int> parse_message_to_vi(const std::string& message);
    void create_board(int players, int board_type);
    void exit_board();

    std::string server_ip;
    int port;
    int sock;
    std::thread receiver_thread;
    bool connected;
    MessageCallback message_callback;
    std::unique_ptr<Board> board;
    std::unordered_map<std::string, std::string> transform_map;
};

#endif // CLIENT_H
