
cmake_minimum_required(VERSION 3.5.1)

project(CGraph)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")

add_executable(CGraph src/main.cpp
        ${SRC_LIST})

