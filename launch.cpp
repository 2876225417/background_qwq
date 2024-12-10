/*#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <filesystem>
#include "utils/image_url_generator.h"

#include <unistd.h>
#include <limits.h>

void print_working_directory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        std::cerr << "Error getting current working directory" << std::endl;
    }
}


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

// 处理 HTTP 请求
void handle_request(boost::beast::http::request<boost::beast::http::basic_string_body<char>> req, boost::asio::ip::tcp::socket& socket) {
    boost::beast::http::response<boost::beast::http::string_body> res;

    if (req.target() == "/api/get_image") {
        // 图片文件路径
        std::string image_path = "./assets/pics/qwq_.jpg"; // 确保路径正确
        std::cout << "Image path: " << image_path << std::endl;

        // 打开图片文件
        std::ifstream file(image_path, std::ios::binary);
        if (!file.is_open()) {
            // 如果文件未找到，返回 404 错误
            std::cerr << "Error opening file: " << image_path << std::endl;
            res.result(boost::beast::http::status::not_found);
            res.body() = "Image not found!";
        } else {
            // 获取文件内容的长度
            file.seekg(0, std::ios::end);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::cout << "Image file size: " << size << " bytes" << std::endl;

            // 读取文件内容
            std::string image_data(size, '\0');
            if (file.read(&image_data[0], size)) {
                // 设置响应头
                res.result(boost::beast::http::status::ok);
                res.set(boost::beast::http::field::content_type, "image/jpeg");
                res.body() = std::move(image_data); // 将图片数据放入响应体
                std::cout << "Response body size: " << res.body().size() << " bytes" << std::endl;
            } else {
                std::cerr << "Error reading image file!" << std::endl;
                res.result(boost::beast::http::status::internal_server_error);
                res.body() = "Failed to read image file!";
            }
        }
    } else {
        // 如果请求的不是目标路径，返回 404 错误
        res.result(boost::beast::http::status::not_found);
        res.body() = "Not Found";
    }

    // 发送响应
    boost::beast::http::write(socket, res);
}




// 处理每个连接
void do_session(tcp::socket&& socket) {
    try {
        beast::flat_buffer buffer;

        // 读取 HTTP 请求
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        // 处理请求
        handle_request(req, socket);

        // 关闭 socket
        socket.shutdown(tcp::socket::shutdown_send);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// 启动 HTTP 服务器
void start_server(const std::string& address, unsigned short port) {
    try {
        net::io_context io_context;
        tcp::endpoint endpoint(net::ip::make_address(address), port);
        tcp::acceptor acceptor(io_context, endpoint);

        std::cout << "Server running on " << address << ":" << port << std::endl;

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);  // 接受连接
            std::thread(do_session, std::move(socket)).detach();  // 异步处理请求
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {

    if (chdir("/www/wwwroot/puoond.space/background_qwq") != 0) {
        std::cerr << "Error changing directory!" << std::endl;
        return 1;
    }

    const std::string address = "127.0.0.1";
    unsigned short port = 8080;

    print_working_directory();

    start_server(address, port);

    return 0;
}*/


#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include "http/http_server.h"
#include "mysql/mysql_connector.h"
#include "mysql/mysql_inserter.h"
#include "mysql/entities.h"

int main() {
    try {
        // 创建数据库连接
        auto db_conn = std::make_shared<database::database_connector>(
            "tcp://127.0.0.1:3306", "background_qwq", "qwq_2004background_1025", "background_qwq");

        // 获取 sql::Connection 对象
        std::shared_ptr<sql::Connection> conn = db_conn->get_connection();

        // 创建插入器，并传递 sql::Connection
        database::database_inserter inserter(conn);

        // 插入一条文章记录
        inserter.insert("articles",
                        "author, title, content, created_at, updated_at, status, slug, excerpt, featured_image, view_count, is_deleted, category, tags",
                        "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?",
                        "Author Name",
                        "Sample Title",
                        "This is the article content.",
                        "2024-12-10 10:00:00",
                        "2024-12-10 10:00:00",
                        "draft",
                        "sample-title",
                        "This is an excerpt",
                        "path/to/featured_image.jpg",
                        0,
                        false,
                        "Technology",
                        "tag1, tag2"
        );

    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

