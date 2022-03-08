#pragma once
#include <array>

namespace kdtree {
	template<class Point>
	struct point_traits {
		using distance_type = Point::distance_type;
		static constexpr auto kdim = Point::kdim;
	};

	template<typename T, auto N>
	struct point_traits<std::array<T, N>> {
		using distance_type = T;
		static constexpr auto kdim = N;
	};
}