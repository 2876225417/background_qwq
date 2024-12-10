#ifndef MYSQL_CONNECTOR_H
#define MYSQL_CONNECTOR_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <memory>
#include <iostream>

namespace database {
    class database_connector {
    public:
        database_connector( const std::string& hst
                          , const std::string& usr
                          , const std::string& pwd
                          , const std::string& db_name
                          );
        ~database_connector();

        bool check_connection();\

               std::shared_ptr<sql::Connection> get_connection() {
            return std::shared_ptr<sql::Connection>(_connection);
        }

        std::unique_ptr<sql::ResultSet> execute_query(const std::string& query);
    private:
          sql::mysql::MySQL_Driver* _driver;
          std::unique_ptr<sql::Connection> _connection;
    };
}         // database
#endif    // MYSQL_CONNECTOR_H