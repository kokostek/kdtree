#include <kdtree/tree.hpp>
#include <vector>
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

using float_vector = std::vector<float>;

int main() {

    // When the number of dimensions (kdim) is known
    // in run-time only, the std::vector can be used
    // as point type for constructing kd-tree.

    // In this case kd::build_tree is not suitable,
    // since it relies on kd::point_kdim_v<T>, which
    // assumes that number of dimensions is known at
    // compile-time.

    // Instead of kd::build_tree we going to use
    // static method kd::tree<T>::build here, which
    // accepts kdim as an argument.

    const int kdim = 2; // assuming this is known at run-time

    // all points need to be at least of size 2, UB otherwise
    const std::vector<float_vector> points{
        {0.26f, -0.91f},
        {-0.38f, -0.53f},
        {0.86f, -0.41f},
        {0.5f, 2.01f},
        {1.26f, -0.44f},
        {-0.35f, 0.46f},
        {-1.67f, -0.86f},
        {0.49f, -0.12f},
        {1.94f, -0.62f},
        {-1.05f, -0.89f},
    };

    const auto tree{
        kd::tree<float_vector>::build(points, kdim)
    };

    // tree is cout-friendly if kd::point_traits<Point>::format is defined
    std::cout << "Tree structure:\n" << tree << "\n";

    print_nearest(tree, { 0.f, 0.f });
    print_nearest(tree, { 0.8f, -0.6f });
    print_nearest(tree, { 100.f, 100.f });

    return EXIT_SUCCESS;
}