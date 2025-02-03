cmake_minimum_required(VERSION 3.10)
project(ServerTests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)

add_executable(server_tests
    test_server.cpp
    test_server_controller.cpp
    test_game.cpp
)

target_include_directories(server_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include  # To access your header files
)

target_link_libraries(server_tests
    PRIVATE
        GTest::GTest
        GTest::Main
        pthread
        ${MYSQLCPPCONN_LIBRARY}
)

target_sources(server_tests 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/board.cpp
        ${CMAKE_SOURCE_DIR}/src/game.cpp
        ${CMAKE_SOURCE_DIR}/src/server_controller.cpp
        ${CMAKE_SOURCE_DIR}/src/server.cpp
        ${CMAKE_SOURCE_DIR}/src/standard_board.cpp
        ${CMAKE_SOURCE_DIR}/src/init_cmds.cpp
        ${CMAKE_SOURCE_DIR}/src/db_connector.cpp
        ${CMAKE_SOURCE_DIR}/src/bot.cpp
        # Don't include main.cpp as it contains the main() function
)

enable_testing()
add_test(
    NAME ServerTests 
    COMMAND server_tests
)
