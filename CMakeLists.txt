
cmake_minimum_required(VERSION 3.5.1)

project(CGraph)

set(CMAKE_CXX_STANDARD 17)

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")
file(GLOB_RECURSE TEST_LIST "./test/*.cpp")

add_library(CGraph SHARED ${SRC_LIST})

add_executable(CGraphTest test/test.cpp
        ${SRC_LIST}
        ${TEST_LIST})