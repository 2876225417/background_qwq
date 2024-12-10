
add_subdirectory(image)




add_library(utils INTERFACE)


target_link_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/image)