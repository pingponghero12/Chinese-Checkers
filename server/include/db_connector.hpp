#ifndef __DBCONNECTOR__
#define __DBCONNECTOR__

#include <mysql/jdbc.h>
#include <memory>


class DbConnector {
public:
    DbConnector(std::string ip, std::string username, std::string password);

    int insert_game(int game_type, int players);
    void insert_move(int game, int move_id, int x1, int y1, int x2, int y2);

    std::vector<std::string> get_games();
    std::vector<int> get_game(int game);
    std::vector<int> get_move(int game, int move_id);

private:
    // Unique since it will only be used in function then we want to destroy it
    std::unique_ptr<sql::ResultSet> request(const std::string& query, const std::vector<std::string>& params);
    void execute(const std::string& query, const std::vector<std::string>& params);
    int execute_insert(const std::string& query, const std::vector<std::string>& params);

    std::unique_ptr<sql::Connection> conn;
    std::string username;
};

#endif // __DBCONNECTOR__
