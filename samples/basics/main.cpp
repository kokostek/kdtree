#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <vector>
#include <initializer_list>
#include <cstdlib>
#include <iostream>

namespace kd = kdtree;

template<kd::point Point>
void print_nearest(const kd::tree<Point>& tree, const Point& key) {
	const auto res = tree.find_nearest(key);
	std::cout 
		<< "Nearest to " << kd::point_traits<Point>::format(key) 
		<< " is " << kd::point_traits<Point>::format(res) << ".\n";
}

void test_int2_tree() {
	std::cout << "****************\n";
	std::cout << "test_int2_tree()\n\n";

	// constructing tree from initializer_list of points
	const auto tree{
		kd::build_tree({
			kd::int2{-4, 9},
			kd::int2{4, 0},
			kd::int2{-3, -4},
			kd::int2{8, 0},
			kd::int2{0, -7},
			kd::int2{-3, -8},
			kd::int2{-9, 1},
			kd::int2{-5, -9},
			kd::int2{-10, 1},
			kd::int2{1, 6},
		})
	};

	// tree is cout-friendly if kd::point_traits<Point>::format is defined
	std::cout << "Tree structure:\n" << tree << "\n";

	print_nearest(tree, { 0, 0 });
	print_nearest(tree, { 9, 1 });
	print_nearest(tree, { 100, 100 });
}

void test_float2_tree() {
	std::cout << "******************\n";
	std::cout << "test_float2_tree()\n\n";

	// constructing tree from std::vector of points,
	// or any other container that satisfies std::ranges::range concept
	std::vector points{
		kd::float2{0.26f, -0.91f},
		kd::float2{-0.38f, -0.53f},
		kd::float2{0.86f, -0.41f},
		kd::float2{0.5f, 2.01f},
		kd::float2{1.26f, -0.44f},
		kd::float2{-0.35f, 0.46f},
		kd::float2{-1.67f, -0.86f},
		kd::float2{0.49f, -0.12f},
		kd::float2{1.94f, -0.62f},
		kd::float2{-1.05f, -0.89f},
	};

	const auto tree{
		kd::build_tree(points)
	};

	// tree is cout-friendly if kd::point_traits<Point>::format is defined
	std::cout << "Tree structure:\n" << tree << "\n";

	print_nearest(tree, { 0.f, 0.f });
	print_nearest(tree, { 0.8f, -0.6f });
	print_nearest(tree, { 100.f, 100.f });
}

int main() {
	test_int2_tree();
	std::cout << "\n";
	test_float2_tree();
	return EXIT_SUCCESS;
}