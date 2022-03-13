#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

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

int main() {
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

    return EXIT_SUCCESS;
}