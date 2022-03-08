#pragma once

#include <array>
#include <cmath>

namespace kdtree {

	template<typename T>
	struct vector2
	{
		using element_type = T;
		static constexpr auto kdims = 2;

		T x, y;

		vector2() : x(T(0)), y(T(0)) {}

		vector2(T x, T y) : x(x), y(y) {}

		T operator[](auto index) const {
			const std::array<T, kdims> xy{ x, y };
			return xy[index];
		}
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const vector2<T>& val) {
		return os << "[" << val.x << ", " << val.y << "]";
	}

	template<typename T>
	inline bool operator==(const vector2<T>& a, const vector2<T>& b) {
		return (a.x == b.x) && (a.y == b.y);
	}

	template<typename T>
	inline bool operator!=(const vector2<T>& a, const vector2<T>& b) {
		return (a.x != b.x) || (a.y != b.y);
	}

	template<typename T>
	inline vector2<T> operator+(const vector2<T>& a, const vector2<T>& b) {
		return { a.x + b.x, a.y + b.y};
	}

	template<typename T>
	inline vector2<T> operator-(const vector2<T>& a, const vector2<T>& b) {
		return { a.x - b.x, a.y - b.y};
	}

	using int2 = vector2<int>;
	using float2 = vector2<float>;
	using double2 = vector2<double>;
}
