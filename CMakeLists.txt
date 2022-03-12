cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (KDTREE)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/node.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/point_traits.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/point2d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/point3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/tree.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kdtree/tree_detail.hpp
)

add_library(kdtree INTERFACE)
target_sources(kdtree INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(kdtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

option(KDTREE_BUILD_TESTS "build tests" ON)
option(KDTREE_BUILD_BENCHMARK "build benchmark" ON)
option(KDTREE_BUILD_SAMPLES "build samples" ON)

if (${KDTREE_BUILD_TESTS})
    add_subdirectory ("test")
endif()

if (${KDTREE_BUILD_BENCHMARK})
    add_subdirectory ("benchmark")
endif()

if (${KDTREE_BUILD_SAMPLES})
    add_subdirectory ("samples/basics")
    add_subdirectory ("samples/custom_point")
    add_subdirectory ("samples/vector_point")
endif()
