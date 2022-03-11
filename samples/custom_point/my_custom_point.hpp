#pragma once
#include <kdtree/point_traits.hpp>
#include <cmath>
#include <ostream>
#include <type_traits>

// uncomment for kdtree::point_traits::format()
//#include <string>
//#include <sstream>

struct my_custom_point {

	float x;
	float y;
	float z;
	float w;

	float operator[](size_t index) const {
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return std::nanf("");
		}
	}
};

// injecting traits for my_custom_point into kdtree namespace
namespace kdtree {
	template<>
	struct point_distance<my_custom_point> {
		using type = float;
	};

	template<>
	struct point_kdim<my_custom_point> {
		static constexpr auto value = 4;
	};

	// For tree to be cout-friendly we need to either override operator<<
	// or to implement kdtree::point_traits::format().
	// Since we implementing operator<<, the format is commented out:
	//template<>
	//struct point_traits<my_custom_point> {
	//	static std::string format(const my_custom_point& p) {
	//		std::ostringstream os;
	//		os << '[' << p.x << ", " << p.y << ", " << p.z << ", " << p.w << ']';
	//		return os.str();
	//	}
	//};
}

std::ostream& operator<<(
	std::ostream& os, const my_custom_point& p) {
	return os << '[' << p.x << ", " << p.y << ", " << p.z << ", " << p.w << ']';
}

static_assert(kdtree::indexable<my_custom_point, float>);
static_assert(std::same_as<kdtree::point_distance_t<my_custom_point>, float>); 
static_assert(kdtree::point_kdim_v<my_custom_point> == 4);
static_assert(kdtree::point<my_custom_point>);