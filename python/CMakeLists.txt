
cmake_minimum_required(VERSION 3.8)
project(PyCGraph)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")

find_package(pybind11 CONFIG REQUIRED)

include(../cmake/CGraph-env-include.cmake)

include_directories(PyCGraph PUBLIC
    ${pybind11_INCLUDE_DIRS}
    )

pybind11_add_module(PyCGraph
    $<TARGET_OBJECTS:CGraph>
    SHARED PyCGraph.cpp
    )

target_link_libraries(PyCGraph PRIVATE
    pybind11::pybind11
    )
