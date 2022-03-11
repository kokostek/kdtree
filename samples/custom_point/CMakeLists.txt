cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
  kdtree_samples_custom_point
  main.cpp
  my_custom_point.hpp)

target_link_libraries(
	kdtree_samples_custom_point
	kdtree
)