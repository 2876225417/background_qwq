set(MYSQL_SRC
    mysql_connector.cpp
    mysql_inserter.cpp
)

add_library(mysql_utils STATIC ${MYSQL_SRC})

target_link_directories(mysql_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})