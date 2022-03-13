# kdtree - header-only [k-d tree](https://en.wikipedia.org/wiki/K-d_tree) implementation

[![Build and test](https://github.com/kokostek/kdtree/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/kokostek/kdtree/actions/workflows/build_and_test.yml)

## Prerequisites
* C++20 compiler for concepts and `std::ranges` support.
* CMake-3.19+ for `CMakePresets.json` support.

## Third-party libraries
* [google test](https://github.com/google/googletest) - for tests only. Turn off `KDTREE_BUILD_TESTS` in cmake to disable tests.
* [google benchmark](https://github.com/google/benchmark) - for benchmarks only. Turn off `KDTREE_BUILD_BENCHMARK` in cmake to disable benchmarks.

## Usage examples

### Built-in point types from `point2d.hpp` or `point3d.hpp`

```c++
#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <iostream>

namespace kd = kdtree;

void main() {
    // constructing tree from initializer_list of points
    const auto tree{
        kd::build_tree({
            kd::int2{-4, 9},
            kd::int2{4, 0},
            kd::int2{-3, -4},
            kd::int2{8, 0},
            kd::int2{0, -7},
            kd::int2{-3, -8},
            kd::int2{-9, 1},
            kd::int2{-5, -9},
            kd::int2{-10, 1},
            kd::int2{1, 6},
        })
    };

    const kd::int2 key{ 9, 1 };
    const kd::int2 res{ tree.find_nearest(key) };

    std::cout << "Nearest to " << kd::format_point(key) << " is " << kd::format_point(res) << "\n";
}
```
Output:
```
Nearest to [9, 1] is [8, 0]
```

### User-defined point types
```c++
#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <iostream>
#include <string>

namespace kd = kdtree;

struct my_custom_point {
    
    float x, y;
    std::string user_data;

    float operator[](size_t index) const {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            return std::nanf("");
        }
    }
};

namespace kdtree {
    template<>
    struct point_distance<my_custom_point> {
        using type = float;
    };

    template<>
    struct point_kdim<my_custom_point> {
        static constexpr auto value = 2;
    };
}

std::ostream& operator<<(
    std::ostream& os, const my_custom_point& p) {
    return os << '[' << p.x << ", " << p.y << ", " << p.user_data << ']';
}

static_assert(kdtree::indexable<my_custom_point, float>);  // accessible by index, with float return type
static_assert(std::same_as<kdtree::point_distance_t<my_custom_point>, float>);  // distance type is float
static_assert(kdtree::point_kdim_v<my_custom_point> == 2);  // number of dims is compile-time constant
static_assert(kdtree::point<my_custom_point>);  // satisfices point concept

void main() {
    const auto tree{
        kd::build_tree({
            my_custom_point{-4, 9, "one"},
            my_custom_point{4, 0, "two"},
            my_custom_point{-3, -4, "three"},
            my_custom_point{8, 0, "four"},
            my_custom_point{0, -7, "five"},
        })
    };

    const my_custom_point key{ 9, 1, "test"};
    const my_custom_point res{ tree.find_nearest(key) };

    std::cout << "Nearest to " << key << " is " << res << "\n";
}
```
Output:
```
Nearest to [9, 1, test] is [8, 0, four]
```

### Point types with run-time number of dimensions
```c++
#include <kdtree/tree.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>

namespace kd = kdtree;

using float_vector = std::vector<float>;

int main() {

    const int kdim = 2; // assuming this is known at run-time

    // all points need to be at least of size 2, UB otherwise
    const std::vector<float_vector> points{
        {0.26f, -0.91f},
        {-0.38f, -0.53f},
        {0.86f, -0.41f},
        {0.5f, 2.01f},
        {-1.05f, -0.89f},
    };

    const auto tree{
        kd::tree<float_vector>::build(points, kdim)
    };

    const float_vector key{ 0.8f, -0.6f };
    const float_vector res{ tree.find_nearest(key) };

    std::cout << "Nearest to " << kd::format_point(key) << " is " << kd::format_point(res) << "\n";

    return EXIT_SUCCESS;
}
```
Output:
```
Nearest to [0.8, -0.6] is [0.86, -0.41]
```

See [samples](https://github.com/kokostek/kdtree/tree/master/samples) for more examples.

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
