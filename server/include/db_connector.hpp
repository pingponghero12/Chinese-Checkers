#ifndef __DBCONNECTOR__
#define __DBCONNECTOR__

#include <mysql/jdbc.h>
#include <memory>


class DbConnector {
public:
    DbConnector(std::string ip, std::string username, std::string password);
private:
  std::unique_ptr<sql::Connection> conn;
};



#endif // __DBCONNECTOR__
