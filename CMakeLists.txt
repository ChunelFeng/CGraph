
cmake_minimum_required(VERSION 3.5.1)

project(CGraph)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")

file(GLOB_RECURSE TEST_LIST "./test/*.cpp")

add_library(CGraph SHARED ${SRC_LIST})

add_executable(CGraphTest test/test.cpp
        ${SRC_LIST}
        ${TEST_LIST})