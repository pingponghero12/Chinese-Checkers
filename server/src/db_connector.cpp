#include "db_connector.hpp"
#include <sstream>

DbConnector::DbConnector(std::string ip, std::string username, std::string password) : username(username) {
    try {
        auto driver = get_driver_instance();
        conn = std::unique_ptr<sql::Connection>(driver->connect(ip, username, password));

        conn->setSchema("cc_db");
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        throw;
    }
}

int DbConnector::insert_game(int game_type, int players) {
    try {
        const std::string query = "INSERT INTO GAMES (game_type, players) "
            "VALUES (?, ?)";

        return execute_insert(query, { std::to_string(game_type), 
                              std::to_string(players)
                              });
    }
    catch(sql::SQLException& e) {
        std::cerr << "Failed to insert game: " << e.what() << std::endl;
        throw;
    }
}

void DbConnector::insert_move(int game, int move_id, int x1, int y1, int x2, int y2) {
    try {
        const std::string query = "INSERT INTO MOVES (game_id, move_id, x1, y1, x2, y2) "
            "VALUES (?, ?, ?, ?, ?, ?)";

        execute(query, { std::to_string(game), 
                              std::to_string(move_id),
                              std::to_string(x1),
                              std::to_string(y1),
                              std::to_string(x2),
                              std::to_string(y2)
                              });
    }
    catch(sql::SQLException& e) {
        std::cerr << "Failed to insert move: " << e.what() << std::endl;
        throw;
    }
}

std::vector<std::string> DbConnector::get_games() {
    std::unique_ptr<sql::ResultSet> result;

    const std::string query = "SELECT game_type, players, created_at FROM GAMES ORDER BY created_at DES LIMIT 20";

    result = request(query, {});

    std::vector<std::string> games;

    while(result->next()) {
        int game_type = result->getInt("game_type");
        int players = result->getInt("players");
        std::string created_at = result->getString("created_at");

        std::stringstream game_info;
        game_info << "Game type: " <<game_type << " Players " << players
            << " Date: " <<created_at;

        games.push_back(game_info.str());
    }

    return games;
}

std::vector<int> DbConnector::get_move(int game, int move_id) {
    std::unique_ptr<sql::ResultSet> result;

    const std::string query = "SELECT x1, y1, x2, y2 FROM MOVES WHERE game_id = ?, move_id = ?";

    result = request(query, {std::to_string(game), std::to_string(move_id)});

    std::vector<int> move;

    while(result->next()) {
        move.push_back(result->getInt("x1"));
        move.push_back(result->getInt("y1"));
        move.push_back(result->getInt("x2"));
        move.push_back(result->getInt("y2"));
    }

    return move;
}

std::unique_ptr<sql::ResultSet> DbConnector::request(const std::string& query, const std::vector<std::string>& params) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(query));

        for(size_t i = 0; i < params.size(); ++i) {
            stmt->setString(i + 1, params[i]);
        }

        return std::unique_ptr<sql::ResultSet>(stmt->executeQuery());
    }
    catch(sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        throw;
    }

}

void DbConnector::execute(const std::string& query, const std::vector<std::string>& params) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(query));

        for(size_t i = 0; i < params.size(); ++i) {
            stmt->setString(i + 1, params[i]);
        }

        stmt->executeUpdate();
    }
    catch(sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        throw;
    }
}

int DbConnector::execute_insert(const std::string& query, const std::vector<std::string>& params) {
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(query));

        for(size_t i = 0; i < params.size(); ++i) {
            stmt->setString(i + 1, params[i]);
        }

        stmt->executeUpdate();

        // Get the last inserted ID
        std::unique_ptr<sql::Statement> id_stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> id_res(id_stmt->executeQuery("SELECT LAST_INSERT_ID()"));

        if (id_res->next()) {
            return id_res->getInt(1);
        }

        throw sql::SQLException("Failed to get last insert ID");
    }
    catch(sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        throw;
    }
}
