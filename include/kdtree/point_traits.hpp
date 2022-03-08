#pragma once
#include <array>
#include <ostream>

namespace kdtree {
	template<class Point>
	struct point_traits {
		using distance_type = Point::distance_type;
		static constexpr auto kdim = Point::kdim;
		
		static constexpr std::ostream& format(std::ostream& os, const Point& p) {
			return os << p;
		}
	};

	template<typename T, auto N>
	struct point_traits<std::array<T, N>> {
		using distance_type = T;
		static constexpr auto kdim = N;

		static constexpr std::ostream& format(std::ostream& os, const std::array<T, N>& p) {
			os << "[";
			for (const auto& t : p) {
				os << t << ", ";
			}
			os << "]";
			return os;
		}
	};
}