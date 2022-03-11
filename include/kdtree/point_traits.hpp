#pragma once
#include <array>
#include <vector>
#include <ostream>
#include <ranges>

namespace kdtree {

	template<typename>
	struct point_kdim;

	template<typename T, auto N>
	struct point_kdim<std::array<T, N>> {
		static constexpr auto value = N;
	};

	template<typename T>
	constexpr auto point_kdim_v = point_kdim<T>::value;

	template<typename>
	struct point_distance;

	template<std::ranges::range T>
	struct point_distance<T> {
		using type = std::ranges::range_value_t<T>;
	};

	template<typename T>
	using point_distance_t = point_distance<T>::type;

	template<class Point>
	struct point_traits {
		static constexpr std::ostream& format(std::ostream& os, const Point& p) {
			return os << p;
		}
	};

	template<std::ranges::range T>
	struct point_traits<T> {
		static constexpr std::ostream& format(std::ostream& os, const T& p) {
			os << "[";
			for (const auto& t : p) {
				os << t << ", ";
			}
			os << "]";
			return os;
		}
	};

	template<class T, class V>
	concept indexable = requires(T t, size_t i) {
		{ t[i] } -> std::convertible_to<V>;
	};

	template<class Point>
	concept point =
		indexable<Point, point_distance_t<Point>>;

	template<class Range>
	concept points_range =
		std::ranges::sized_range<Range> &&
		point<std::ranges::range_value_t<Range>>;

	template<points_range Points>
	using points_range_point_t = std::ranges::range_value_t<Points>;
	
	static_assert(std::same_as<point_distance_t<std::vector<int>>, int>);
	static_assert(std::same_as<point_distance_t<std::vector<float>>, float>);
	static_assert(std::same_as<point_distance_t<std::vector<double>>, double>);
}