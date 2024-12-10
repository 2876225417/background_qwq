
#include "mysql_connector.h"


namespace database {
    database_connector::database_connector( const std::string& hst
                                          , const std::string& usr
                                          , const std::string& pwd
                                          , const std::string& db_name
                                          )
        : _driver(sql::mysql::get_mysql_driver_instance())
        , _connection(std::unique_ptr<sql::Connection>(_driver->connect(hst, usr, pwd))) {
        _connection->setSchema(db_name);
    }

    bool database_connector::check_connection() {
        try {
            return true;
        } catch (const sql::SQLException& e) {
            std::cerr << "Database connection error: " << e.what() << '\n';
            return false;
        }
    }

    std::unique_ptr<sql::ResultSet> database_connector::execute_query(const std::string& query) {
        try {
            std::unique_ptr<sql::Statement> stmt(_connection->createStatement());
            return std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
        } catch (const sql::SQLException& e) {
            std::cerr << "SQL error: " << e.what() << '\n';
            return nullptr;
        }
    }
}   // database