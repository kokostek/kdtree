#pragma once
#include <array>
#include <cmath>

#include "tree.h"

namespace kdtree {
	const size_t KDimsFloat2 = 2;

	struct float2;

	using float2_node = node<float2>;
	using float2_tree = tree<float2, float, KDimsFloat2>;

#pragma pack(push, 1)
	struct float2 {

		float x;
		float y;

		const float& operator[](size_t index) const {
			auto f = reinterpret_cast<const float*>(this);
			return f[index];
		}

		float& operator[](size_t index) {
			auto f = reinterpret_cast<float*>(this);
			return f[index];
		}

		float dot(const float2& other) const {
			return x * other.x + y * other.y;
		}

		float abs_sqr() const {
			return x * x + y * y;
		}

		float abs() const {
			return std::sqrtf(abs_sqr());
		}
	};
#pragma pack(pop)

	std::ostream& operator<<(std::ostream& os, const float2& val) {
		return os << "(" << val.x << ", " << val.y << ")";
	}

	bool operator==(const float2& a, const float2& b) {
		return (a.x == b.x) && (a.y == b.y);
	}

	bool operator!=(const float2& a, const float2& b) {
		return (a.x != b.x) || (a.y != b.y);
	}

	float2 operator+(const float2& a, const float2& b) {
		return { a.x + b.x, a.y + b.y };
	}

	float2 operator-(const float2& a, const float2& b) {
		return { a.x - b.x, a.y - b.y };
	}
}
