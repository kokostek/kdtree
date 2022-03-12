# kdtree - header-only [k-d tree](https://en.wikipedia.org/wiki/K-d_tree) implementation

[![Build and test](https://github.com/kokostek/kdtree/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/kokostek/kdtree/actions/workflows/build_and_test.yml)

## Prerequisites
* C++20 compiler for concepts and `std::ranges` support.
* CMake-3.19+ for `CMakePresets.json` support.

See [samples](https://github.com/kokostek/kdtree/tree/master/samples) dir for usage examples.

## Third-party libraries
* [google test](https://github.com/google/googletest) - for tests only. Turn off `KDTREE_BUILD_TESTS` in cmake to disable tests.
* [google benchmark](https://github.com/google/benchmark) - for benchmarks only. Turn off `KDTREE_BUILD_BENCHMARK` in cmake to disable benchmarks.

## Benchmark results for Intel Core i9-9900K

Tree build time depending on points count:
```
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
SpherialClouds/tree_build/1024               159465 ns       157286 ns         4073
SpherialClouds/tree_build/4096               740411 ns       732422 ns          896
SpherialClouds/tree_build/32768             6991033 ns      6944444 ns           90
SpherialClouds/tree_build/131072           32953119 ns     33482143 ns           21
```

Baseline search time for for naive O(M*N) search:
```
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
SpherialClouds/baseline_tree_find/1024      1094658 ns      1074219 ns          640
SpherialClouds/baseline_tree_find/4096      4286931 ns      4296875 ns          160
SpherialClouds/baseline_tree_find/32768    34170335 ns     34375000 ns           20
SpherialClouds/baseline_tree_find/131072  134350217 ns    135416667 ns            6
```

Search time for k-d tree with different point cloud configurations:
* overlapping spherical clouds, both tree and search points ar from normal distributions N([0, 0], [1, 1]) along all dimensions ([0, 0] is the center of point cloud, and [1, 1] is normal distribution sigma along all dimensions)
```
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
SpherialClouds/tree_find/1024                337878 ns       337672 ns         2036
SpherialClouds/tree_find/4096                394814 ns       392369 ns         1792
SpherialClouds/tree_find/32768               526110 ns       515625 ns         1000
SpherialClouds/tree_find/131072              591995 ns       585938 ns         1120
```
* distant spherical clouds: tree points are from N([10, 10], [1, 1]), search points are from N([0, 0], [1, 1])
```
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
DistantSpherialClouds/tree_find/1024        8697351 ns      8541667 ns           75
DistantSpherialClouds/tree_find/4096       33705075 ns     33593750 ns           20
DistantSpherialClouds/tree_find/32768     180077600 ns    179687500 ns            4
DistantSpherialClouds/tree_find/131072    483892750 ns    484375000 ns            2
```
* overlapping parallel lines: both tree and search points are from N([0, 0], [1, 0.1])
```
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
ParallelLines/tree_find/1024                 383507 ns       383650 ns         1792
ParallelLines/tree_find/4096                 452579 ns       455097 ns         1545
ParallelLines/tree_find/32768                588825 ns       599888 ns         1120
ParallelLines/tree_find/131072               651693 ns       655692 ns         1120
```
* intersecting normal lines: tree points are from N([0, 0], [0.1, 1]), search points are from N([0, 0], [1, 0.1])
```
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
NormalLines/tree_find/1024                   635327 ns       627790 ns         1120
NormalLines/tree_find/4096                  1039128 ns      1045850 ns          747
NormalLines/tree_find/32768                 2462831 ns      2455357 ns          280
NormalLines/tree_find/131072                4530036 ns      4614094 ns          149
```
