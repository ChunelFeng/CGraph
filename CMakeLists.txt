
cmake_minimum_required(VERSION 3.5.1)

project(CGraph)

set(CMAKE_CXX_STANDARD 17)

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

IF(APPLE)
    # 非mac平台，暂时不支持自动生成session信息
    set(CMAKE_CXX_FLAGS -m64)
    add_definitions(-D_GENERATE_SESSION_)
ELSEIF(UNIX)
    # linux平台加入多线程内容
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -O2 -pthread -Wno-format-overflow")
ENDIF()

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")

set(TUTORIAL_LIST
        T01-Simple
        T02-Cluster
        T03-Region
        T04-Complex
        T05-Param
        T06-Condition)

# 编译libCGraph动态库
add_library(CGraph SHARED ${SRC_LIST})

foreach(tut ${TUTORIAL_LIST})
    add_executable(${tut} tutorial/${tut}.cpp ${SRC_LIST})
endforeach()

