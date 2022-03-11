#pragma once

#include "point_traits.hpp"
#include <array>

namespace kdtree {
    template<typename T>
    using point2d = std::array<T, 2>;

    using int2 = point2d<int>;
    using float2 = point2d<float>;
    using double2 = point2d<double>;

    static_assert(point<int2>);
    static_assert(point<float2>);
    static_assert(point<double2>);

    static_assert(point_kdim_v<int2> == 2);
    static_assert(point_kdim_v<float2> == 2);
    static_assert(point_kdim_v<double2> == 2);

    static_assert(std::same_as<point_distance_t<int2>, int>);
    static_assert(std::same_as<point_distance_t<float2>, float>);
    static_assert(std::same_as<point_distance_t<double2>, double>);
}
