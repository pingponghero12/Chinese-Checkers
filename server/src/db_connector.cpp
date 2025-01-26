#include "db_connector.hpp"

DbConnector::DbConnector(std::string ip, std::string username, std::string password) {
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
