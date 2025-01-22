cmake_minimum_required(VERSION 3.10)
project(Server CXX)

#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_COMPILER "/nix/store/xcn9p4xxfbvlkpah7pwchpav4ab9d135-gcc-wrapper-14-20241116/bin/g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(server ${SOURCES})

add_subdirectory(tests)
