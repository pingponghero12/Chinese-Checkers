cmake_minimum_required(VERSION 3.10)
project(Client CXX)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(client ${SOURCES})
