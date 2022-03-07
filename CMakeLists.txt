# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (KDTREE)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/float2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/node.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/tree.hpp)

add_library(kdtree INTERFACE)
target_sources(kdtree INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(kdtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

# Include sub-projects.
add_subdirectory ("test")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)