#pragma once

#include <array>
#include <array>
#include <cmath>
#include <ostream>

namespace kdtree {
	template<typename T>
	using point2d = std::array<T, 2>;

	using int2 = point2d<int>;
	using float2 = point2d<float>;
	using double2 = point2d<double>;
}
