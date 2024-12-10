#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <memory>
#include <string>
#include "../mysql/mysql_connector.h"

using boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace http = beast::http;


class http_session: public std::enable_shared_from_this<http_session> {
public:
    explicit http_session( tcp::socket socket
                         , std::shared_ptr<database::database_connector> db_conn
                         );
    void start();

private:
    void do_read();
    void handle_request();

    tcp::socket                                   _socket;
    beast::flat_buffer                            _buffer;
    http::request<http::string_body>              _request;
    std::shared_ptr<database::database_connector> _db_conn;
};


#endif
