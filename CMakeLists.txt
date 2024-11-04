
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")
message("*     ______   ______                             __    *")
message("*    / ____/  / ____/   _____  ____ _    ____    / /_   *")
message("*   / /      / / __    / ___/ / __ `/   / __ \\  / __ \\  *")
message("*  / /___   / /_/ /   / /    / /_/ /   / /_/ / / / / /  *")
message("*  \\____/   \\____/   /_/     \\__,_/   / .___/ /_/ /_/   *")
message("*                                    /_/ by Chunel Feng *")
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")

cmake_minimum_required(VERSION 3.5.0)

project(CGraph VERSION 2.6.1)

# CGraph默认使用C++11版本，推荐使用C++17版本。暂不支持C++11以下版本
set(CMAKE_CXX_STANDARD 11)

option(CGRAPH_BUILD_FUNCTIONAL_TESTS "Enables builds of functional tests" OFF)
option(CGRAPH_BUILD_PERFORMANCE_TESTS "Enables builds of performance tests" OFF)

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

# 如果开启此宏定义，则在结束过程中，会展示thread pool中的调度信息
# 本信息主要用于作者性能调优使用，不保证所有数据完全正确
# 不建议作为验证自行逻辑的唯一标准，不建议使用者打开
# add_definitions(-D_CGRAPH_SHOW_THREAD_METRICS_)

# 此宏可以在纯并发的微小任务下，用于提升整体性能。主要用于在性能测试的情况下使用，一般情况不推荐打开
# add_definitions(-D_CGRAPH_PARALLEL_MICRO_BATCH_ENABLE_)

# add CGraph environment info
include(cmake/CGraph-env-include.cmake)

add_subdirectory(./tutorial)
add_subdirectory(./example)

# 功能测试相关内容
if(CGRAPH_BUILD_FUNCTIONAL_TESTS)
    message(STATUS "[CGraph] build functional tests")
    add_subdirectory(./test/Functional)
endif(CGRAPH_BUILD_FUNCTIONAL_TESTS)

# 性能测试相关内容
if(CGRAPH_BUILD_PERFORMANCE_TESTS)
    message(STATUS "[CGraph] build performance tests")
    add_subdirectory(./test/Performance)
endif(CGRAPH_BUILD_PERFORMANCE_TESTS)
