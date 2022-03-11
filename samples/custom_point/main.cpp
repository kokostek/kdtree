#include "my_custom_point.hpp"

#include <kdtree/tree.hpp>
#include <vector>
#include <initializer_list>
#include <cstdlib>
#include <iostream>

namespace kd = kdtree;

template<kd::point Point>
void print_nearest(const kd::tree<Point>& tree, const Point& key) {
    const auto res = tree.find_nearest(key);
    std::cout 
        << "Nearest to " << kd::point_traits<Point>::format(key) 
        << " is " << kd::point_traits<Point>::format(res) << ".\n";
}

int main() {
    // build_tree cam be used here since we injected point_kdim<my_custom_point>
    const auto tree{
        kd::build_tree({
            my_custom_point{-0.75f, 1.32f, 1.25f, -1.6f},
            my_custom_point{-1.47f, -1.72f, 1.86f, 0.09f},
            my_custom_point{-0.05f, 0.56f, -0.96f, -0.18f},
            my_custom_point{-1.18f, 0.61f, -0.95f, 0.36f},
            my_custom_point{1.06f, -0.12f, 0.83f, -1.21f},
            my_custom_point{-1.19f, 0.22f, -0.21f, -1.41f},
            my_custom_point{-0.48f, 1.2f, -0.71f, 0.03f},
            my_custom_point{-0.39f, 0.19f, 0.35f, 0.59f},
            my_custom_point{0.52f, 0.83f, 0.43f, 0.2f},
            my_custom_point{-0.51f, -3.07f, -0.4f, -0.8f},
        })
    };

    print_nearest(tree, { 0, 0, 0, 0 });
    print_nearest(tree, { -0.5f, 0.25f, 0.5f, -0.25f });
    print_nearest(tree, { 100, 100, 100, 100 });

    return EXIT_SUCCESS;
}