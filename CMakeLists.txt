
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")
message("*     ______   ______                             __    *")
message("*    / ____/  / ____/   _____  ____ _    ____    / /_   *")
message("*   / /      / / __    / ___/ / __ `/   / __ \\  / __ \\  *")
message("*  / /___   / /_/ /   / /    / /_/ /   / /_/ / / / / /  *")
message("*  \\____/   \\____/   /_/     \\__,_/   / .___/ /_/ /_/   *")
message("*                                    /_/ by Chunel Feng *")
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")

cmake_minimum_required(VERSION 3.1.0)

project(CGraph VERSION 2.6.0)

# CGraph默认使用C++11版本，推荐使用C++17版本。暂不支持C++11以下版本
set(CMAKE_CXX_STANDARD 11)

option(CGRAPH_BUILD_FUNCTIONAL_TESTS "Enables builds of functional tests" OFF)
option(CGRAPH_BUILD_PERFORMANCE_TESTS "Enables builds of performance tests" OFF)

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

# add CGraph environment info
include(cmake/CGraph-env-include.cmake)

add_subdirectory(./tutorial)
add_subdirectory(./example)

# 功能测试相关内容
if(CGRAPH_BUILD_FUNCTIONAL_TESTS)
    message("-- [CGraph] build functional tests")
    add_subdirectory(./test/Functional)
endif(CGRAPH_BUILD_FUNCTIONAL_TESTS)

# 性能测试相关内容
if(CGRAPH_BUILD_PERFORMANCE_TESTS)
    message("-- [CGraph] build performance tests")
    add_subdirectory(./test/Performance)
endif(CGRAPH_BUILD_PERFORMANCE_TESTS)
