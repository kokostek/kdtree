#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <iostream>
#include <cstdlib>

namespace kd = kdtree;

int main() {
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
    
    return EXIT_SUCCESS;
}