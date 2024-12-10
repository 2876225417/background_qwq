cmake_minimum_required(VERSION 3.12)

set(VCPKG_ROOT "/www/wwwroot/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(test)

set(Boost_DIR "/www/wwwroot/vcpkg/installed/x64-linux/share/boost")

include_directories(/usr/include/cppconn)
link_directories(/usr/lib/x86_64-linux-gun)
find_package(Boost REQUIRED asio system beast)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE Boost::asio Boost::system Boost::beast mysqlcppconn)