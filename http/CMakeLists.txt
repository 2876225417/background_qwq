

set(HTTP_SRC
    http_server.cpp
    http_session.cpp
)

add_library(http STATIC ${HTTP_SRC})

target_link_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})