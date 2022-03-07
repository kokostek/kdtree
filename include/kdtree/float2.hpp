#pragma once

#include <array>
#include <cmath>

namespace kdtree {
	struct float2 {

		float x, y;

		float2() : x(0.f), y(0.f) {}

		float2(float x, float y) : x(x), y(y) {}

		const float& operator[](auto index) const {
			const std::array<float, 2> xy{ x, y };
			return xy[index];
		}

		float& operator[](auto index) {
			const std::array<float, 2> xy{ x, y };
			return xy[index];
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const float2& val) {
		return os << "(" << val.x << ", " << val.y << ")";
	}

	inline bool operator==(const float2& a, const float2& b) {
		return (a.x == b.x) && (a.y == b.y);
	}

	inline bool operator!=(const float2& a, const float2& b) {
		return (a.x != b.x) || (a.y != b.y);
	}

	inline float2 operator+(const float2& a, const float2& b) {
		return { a.x + b.x, a.y + b.y};
	}

	inline float2 operator-(const float2& a, const float2& b) {
		return { a.x - b.x, a.y - b.y};
	}
}
