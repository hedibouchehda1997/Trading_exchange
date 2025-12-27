add_library(common STATIC)

target_sources(common
    PRIVATE
        src/logging.cpp
        src/sockets.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)