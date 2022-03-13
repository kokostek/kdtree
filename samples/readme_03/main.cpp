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