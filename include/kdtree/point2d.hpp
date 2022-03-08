#pragma once

#include <array>
#include <cmath>
#include <ostream>

namespace kdtree {
	template<typename T>
	struct point2d {
		using distance_type = T;
		static constexpr auto kdim = 2;

		T x, y;

		point2d() : x(T(0)), y(T(0)) {}

		point2d(T x, T y) : x(x), y(y) {}

		T operator[](auto index) const {
			const std::array<T, kdim> xy{ x, y };
			return xy[index];
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const point2d<T>& val) {
		return os << "[" << val.x << ", " << val.y << "]";
	}

	template<typename T>
	inline bool operator==(const point2d<T>& a, const point2d<T>& b) {
		return (a.x == b.x) && (a.y == b.y);
	}

	template<typename T>
	inline bool operator!=(const point2d<T>& a, const point2d<T>& b) {
		return (a.x != b.x) || (a.y != b.y);
	}

	template<typename T>
	inline point2d<T> operator+(const point2d<T>& a, const point2d<T>& b) {
		return { a.x + b.x, a.y + b.y};
	}

	template<typename T>
	inline point2d<T> operator-(const point2d<T>& a, const point2d<T>& b) {
		return { a.x - b.x, a.y - b.y};
	}

	using int2 = point2d<int>;
	using float2 = point2d<float>;
	using double2 = point2d<double>;
}
