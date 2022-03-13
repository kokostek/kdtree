cmake_minimum_required (VERSION 3.8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
  kdtree_samples_readme_02
  main.cpp)

target_link_libraries(
	kdtree_samples_readme_02
	kdtree
)