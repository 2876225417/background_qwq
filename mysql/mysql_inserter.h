




#ifndef MYSQL_INSERTER_H
#define MYSQL_INSERTER_H

#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <memory>
#include <iostream>
#include <string>
#include <map>

namespace database {
    class database_inserter {
    public:
        explicit database_inserter(std::shared_ptr<sql::Connection> connection);
        ~database_inserter();

        template <typename... Args>
        bool insert( const std::string& table
                   , const std::string& columns
                   , const std::string& values
                   , Args&&... args
                   );
    private:
        template <typename T, typename... Args>
        void set_parameters( std::unique_ptr<sql::PreparedStatement>& pstmt
                          , T&& param
                          , Args&&... args);

        void set_parameters(std::unique_ptr<sql::PreparedStatement>& pstmt);
        std::shared_ptr<sql::Connection> _connection;
    };
}
#endif