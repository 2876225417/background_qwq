

/* show a instance at a second git changes*/

#include "mysql_inserter.h"

namespace database {
    database_inserter::database_inserter(std::shared_ptr<sql::Connection> connection)
        : _connection(connection) {}

    database_inserter::~database_inserter() {}

    template <typename... Args>
    bool database_inserter::insert( const std::string& table
                                  , const std::string& columns
                                  , const std::string& values
                                  , Args&&... args) {
                                        try {
                                                
                                            
                                                std::string query = "INSERT INTO " + table + " (" + columns + ") VALUES (" + values + ")";
                                                std::unique_ptr<sql::PreparedStatement> pstmt(_connection->prepareStatement(query));

                                                set_parameters(pstmt, std::forward<Args>(args)...);
                                                pstmt->executeUpdate();
                                                std::cout << "Record inserted successfully!\n";
                                                return true;
                                             } catch (const sql::SQLException& e) {
                                                std::cerr << "Error inserting record: " << e.what() << '\n';
                                                return false;
                                             }
                                  }

    template <typename T, typename... Args>
    void database_inserter::set_parameters( std::unique_ptr<sql::PreparedStatement>& pstmt
                                          , T&& param
                                          , Args&&... args) {
                                                set_parameter(pstmt, sizeof...(args) + 1, std::forward<T>(param));


                                                set_parameters(pstmt, std::forward<Args>(args)...);
                                          }

    void database_inserter::set_parameters(std::unique_ptr<sql::PreparedStatement>& pstmt) {}
    /* this is test instance...*/
}