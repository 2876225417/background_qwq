cmake_minimum_required(VERSION 3.12)

set(VCPKG_ROOT "/www/wwwroot/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "{CMAKE_CXX_FLAGS_DEBUG}" -g -O0)
endif()

project(background_qwq)

set(Boost_DIR "/www/wwwroot/vcpkg/installed/x64-linux/share/boost")

include_directories(/usr/include/cppconn)
link_directories(/usr/lib/x86_64-linux-gun)

find_package(Boost REQUIRED asio system beast)

add_subdirectory(utils)
add_subdirectory(http)
add_subdirectory(mysql)

set(SOURCE_FILES
    launch.cpp
)

add_executable(background_qwq ${SOURCE_FILES})

target_link_libraries(background_qwq PRIVATE Boost::asio Boost::system Boost::beast mysqlcppconn pthread http mysql_utils utils)





install(TARGETS background_qwq DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/utils DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/articles DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/assets DESTINATION assets)