
#include "http_server.h"

http_server::http_server( boost::asio::io_context& io_context
                        , short port
                        , std::shared_ptr<database::database_connector> db_conn
                        )
    : _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    , _db_conn(std::move(db_conn))
{
    do_accept();
}



void http_server::do_accept() {
    _acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if(!ec) std::make_shared<http_session>(std::move(socket), _db_conn)->start();
          do_accept();
    });
}



