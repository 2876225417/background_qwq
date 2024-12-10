
set(IMAGE_SRC
    image_url_generator.cpp
)

add_library(image STATIC ${IMAGE_SRC})

target_link_directories(image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
