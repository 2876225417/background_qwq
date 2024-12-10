
#include "http_session.h"

http_session::http_session( tcp::socket socket
                          , std::shared_ptr<database::database_connector> db_conn
                          )
    : _socket(std::move(socket))
    , _db_conn(std::move(db_conn)) {}

void http_session::start() {
    do_read();
}

void http_session::do_read() {
    auto self(shared_from_this());
    http::async_read(_socket, _buffer, _request,
                    [this, self](boost::system::error_code ec, std::size_t) {
                       if(!ec) handle_request();
                    });
}

void http_session::handle_request() {
    http::response<http::string_body> response;
    if(_request.target() == "api/hello") {
        response.result(http::status::ok);
        response.set(http::field::content_type, "text/plain");
        response.body() = "Hello from asio!";
    } else if (_request.target() == "api/dbstatus") {
        if(_db_conn->check_connection()) {
            response.result(http::status::ok);
            response.set(http::field::content_type, "text/plain");
            response.body() = "Database connection successful!";
        } else {
            response.result(http::status::internal_server_error);
            response.set(http::field::content_type, "text/plain");
            response.body() = "Database connection failed!";
        }
    } else {
        response.result(http::status::not_found);
        response.set(http::field::content_type, "text/plain");
        response.body() = "Not Found";
    }

    auto self(shared_from_this());
    http::async_write(_socket, response,
                     [this, self](boost::system::error_code ec, std::size_t) {
                         _socket.shutdown(tcp::socket::shutdown_send, ec);
                     });

}