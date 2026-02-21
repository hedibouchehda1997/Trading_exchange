add_library(matching_engine STATIC)

target_sources(matching_engine
    PRIVATE
        src/matching_engine.cpp
)

target_link_libraries(matching_engine
    PUBLIC
        common
)

target_include_directories(matching_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
         ${CMAKE_SOURCE_DIR}/../common/include
)