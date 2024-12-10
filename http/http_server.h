#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>
#include "http_session.h"
#include "../mysql/mysql_connector.h"

using boost::asio::ip::tcp;

class http_server {
public:
    http_server( boost::asio::io_context& io_context
               , short port
               , std::shared_ptr<database::database_connector> db_conn
               );
private:
    void do_accept();

    tcp::acceptor _acceptor;
    std::shared_ptr<database::database_connector> _db_conn;
};
#endif