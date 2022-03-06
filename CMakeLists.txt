# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (KDTREE)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/float2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/node.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/tree.hpp)

add_library(type_safe INTERFACE)
target_sources(type_safe INTERFACE "$<BUILD_INTERFACE:${header_files}>")

# Include sub-projects.
add_subdirectory ("test")
