
cmake_minimum_required(VERSION 3.8)
project(pyCGraph)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -pthread -fPIC")

find_package(PythonLibs REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

include(../cmake/CGraph-env-include.cmake)

include_directories(pyCGraph PUBLIC
    ${pybind11_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS})

pybind11_add_module(pyCGraph
    $<TARGET_OBJECTS:CGraph>
    SHARED pyCGraph.cpp)

target_link_libraries(pyCGraph PRIVATE
    pybind11::pybind11
    ${PYTHON_LIBRARIES})
