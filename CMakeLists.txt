
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")
message("*     ______   ______                             __    *")
message("*    / ____/  / ____/   _____  ____ _    ____    / /_   *")
message("*   / /      / / __    / ___/ / __ `/   / __ \\  / __ \\  *")
message("*  / /___   / /_/ /   / /    / /_/ /   / /_/ / / / / /  *")
message("*  \\____/   \\____/   /_/     \\__,_/   / .___/ /_/ /_/   *")
message("*                                    /_/ by Chunel Feng *")
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")

cmake_minimum_required(VERSION 3.5.0)

project(CGraph VERSION 2.3.4)

# CGraph默认使用C++11版本，推荐使用C++17版本。暂不支持C++11以下版本
set(CMAKE_CXX_STANDARD 11)

# add CGraph environment info
include(cmake/CGraph-env-include.cmake)

file(GLOB_RECURSE CGRAPH_SRC_LIST "./src/*.cpp")

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

# 以下三选一，本地编译执行，推荐OBJECT方式
add_library(CGraph OBJECT ${CGRAPH_SRC_LIST})      # 通过代码编译
# add_library(CGraph SHARED ${CGRAPH_SRC_LIST})    # 编译libCGraph动态库
# add_library(CGraph STATIC ${CGRAPH_SRC_LIST})    # 编译libCGraph静态库

# 加入 tutorial中的可执行的例子
add_subdirectory(tutorial)
