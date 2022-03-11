#pragma once

#include "point_traits.hpp"
#include <array>

namespace kdtree {
    template<typename T>
    using point3d = std::array<T, 3>;

    using int3 = point3d<int>;
    using float3 = point3d<float>;
    using double3 = point3d<double>;

    static_assert(point<int3>);
    static_assert(point<float3>);
    static_assert(point<double3>);

    static_assert(point_kdim_v<int3> == 3);
    static_assert(point_kdim_v<float3> == 3);
    static_assert(point_kdim_v<double3> == 3);

    static_assert(std::same_as<point_distance_t<int3>, int>);
    static_assert(std::same_as<point_distance_t<float3>, float>);
    static_assert(std::same_as<point_distance_t<double3>, double>);
}
