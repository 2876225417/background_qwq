#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <iostream>
#include <string>
#include <memory>  // For std::shared_ptr

using boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace http = beast::http;

// 检查数据库连接的函数
bool check_database_connection() {
    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        // 获取 MySQL 驱动实例
        driver = sql::mysql::get_mysql_driver_instance();
        // 创建数据库连接
        con = driver->connect("tcp://127.0.0.1:3306", "background_qwq", "qwq_2004background_1025");
        
        // 选择数据库
        con->setSchema("background_qwq");

        // 执行一个简单查询来确认连接是否成功
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT 1"));

        // 如果查询结果正常，则连接成功
        if (res->next()) {
            delete con;
            return true;
        }
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "Database connection error: " << e.what() << "\n";
    }
    return false;
}

// HttpSession 类，处理 HTTP 请求会话
class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    explicit HttpSession(tcp::socket socket)
            : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        http::async_read(socket_, buffer_, request_,
                         [this, self](boost::system::error_code ec, std::size_t) {
                             if (!ec) {
                                 handle_request();
                             }
                         });
    }

    void handle_request() {
        // 创建响应
        http::response<http::string_body> response;

        if (request_.target() == "/api/hello") {
            response.result(http::status::ok);
            response.set(http::field::content_type, "text/plain");
            response.body() = "hello from asio!";
        }
        // 检查数据库连接状态的 API
        else if (request_.target() == "/api/dbstatus") {
            if (check_database_connection()) {
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

        // 发送响应
        auto self(shared_from_this());
        http::async_write(socket_, response,
                          [this, self](boost::system::error_code ec, std::size_t) {
                              if (!ec) {
                                  socket_.shutdown(tcp::socket::shutdown_send, ec);
                              }
                          });
    }

    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> request_;
};

// HttpServer 类，处理接受连接
class HttpServer {
public:
    HttpServer(boost::asio::io_context& io_context, short port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        // 这里使用 std::make_shared<HttpSession> 创建并启动会话
                        std::make_shared<HttpSession>(std::move(socket))->start();
                    }
                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        // 启动 HttpServer，监听端口 8080
        HttpServer server(io_context, 8080);

        // 启动 IO 服务
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
