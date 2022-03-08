#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <gtest/gtest.h>
#include <vector>

namespace kd = kdtree;

TEST(tree, empty) {
	kd::tree_t<kd::float2> tree;
	EXPECT_FALSE(tree.root());
}

TEST(tree, single_node) {
	const auto tree = kd::make_tree<kd::float2>(kd::make_node(kd::float2{ 1, 2 }));
	EXPECT_TRUE(tree.root());
	EXPECT_EQ(tree.root()->value(), (kd::float2{ 1, 2 }));
	EXPECT_FALSE(tree.root()->left(), kd::float2{ 1, 2 });
	EXPECT_FALSE(tree.root()->right(), kd::float2{ 1, 2 });
}

TEST(tree, find_nearest_no_throw) {
	const auto tree = kd::make_tree<kd::float2>(
		kd::make_node(
			kd::float2{ 0, 0 },
			kd::make_node(
				kd::float2{ 1, 2 },
				kd::make_node(kd::float2{ 3, 4 }),
				kd::make_node(kd::float2{ 5, 6 })),
			kd::make_node<kd::float2>(
				kd::float2{ 7, 8 },
				kd::make_node(kd::float2{ 9, 10 }),
				kd::make_node(kd::float2{ 11, 12 })))
	);

	EXPECT_NO_THROW(tree.find_nearest(kd::float2{ 1.5f, 0.5f }));
}

TEST(tree, equality) {
	const auto tree1 = kd::make_tree<kd::float2>(
		kd::make_node(
			kd::float2{ 7, 2 },
			kd::make_node(
				kd::float2{ 5, 4 },
				kd::make_node(kd::float2{ 2, 3 }),
				kd::make_node(kd::float2{ 4, 7 })
			),
			kd::make_node(
				kd::float2{ 9, 6 },
				kd::make_node(kd::float2{ 8, 1 }),
				kd::make_leaf<kd::float2>()
			)
		)
	);

	const auto tree2 = kd::make_tree<kd::float2>(
		kd::make_node(
			kd::float2{ 7, 2 },
			kd::make_node(
				kd::float2{ 5, 4 },
				kd::make_node(kd::float2{ 2, 3 }),
				kd::make_node(kd::float2{ 4, 7 })
			),
			kd::make_node(
				kd::float2{ 9, 6 },
				kd::make_node(kd::float2{ 8, 1 }),
				kd::make_leaf<kd::float2>()
			)
		)
	);

	const auto tree3 = kd::make_tree<kd::float2>(
		kd::make_node(
			kd::float2{ 7, 2 },
			kd::make_node(
				kd::float2{ 5, 4 },
				kd::make_node(kd::float2{ 4, 1 }),
				kd::make_node(kd::float2{ 4, 7 })
			),
			kd::make_node(
				kd::float2{ 9, 6 },
				kd::make_node(kd::float2{ 8, 1 }),
				kd::make_leaf<kd::float2>()
			)
		)
	);

	const kd::tree_t<kd::float2> tree4;

	EXPECT_TRUE(tree1 == tree1);
	EXPECT_TRUE(tree1 == tree2);
	EXPECT_TRUE(tree1 != tree3);
	EXPECT_TRUE(tree1 != tree4);

	EXPECT_TRUE(tree2 == tree1);
	EXPECT_TRUE(tree2 == tree2);
	EXPECT_TRUE(tree2 != tree3);
	EXPECT_TRUE(tree2 != tree4);

	EXPECT_TRUE(tree3 != tree1);
	EXPECT_TRUE(tree3 != tree2);
	EXPECT_TRUE(tree3 == tree3);
	EXPECT_TRUE(tree3 != tree4);

	EXPECT_TRUE(tree4 != tree1);
	EXPECT_TRUE(tree4 != tree2);
	EXPECT_TRUE(tree4 != tree3);
	EXPECT_TRUE(tree4 == tree4);
}

TEST(tree, build) {
	const std::vector<kd::float2> points{
		{2, 3},
		{5, 4},
		{9, 6},
		{4, 7},
		{8, 1},
		{7, 2},
	};

	const auto expected_tree = kd::make_tree<kd::float2>(
		kd::make_node(
			kd::float2{7, 2},
			kd::make_node(
				kd::float2{ 5, 4 },
				kd::make_node(kd::float2{ 2, 3 }),
				kd::make_node(kd::float2{ 4, 7 })
			),
			kd::make_node(
				kd::float2{ 9, 6 },
				kd::make_node(kd::float2{ 8, 1 }),
				kd::make_leaf<kd::float2>()
			)
		)
	);

	const auto actual_tree{ kd::build_tree<kd::float2>(points) };

	EXPECT_EQ(expected_tree, actual_tree);
}

TEST(tree, build_vector) {
	const std::vector<std::vector<float>> points{
		{2, 3},
		{5, 4},
		{9, 6},
		{4, 7},
		{8, 1},
		{7, 2},
	};

	const auto expected_tree = kd::tree_t<std::vector<float>>(
		kd::make_node(
			std::vector{ 7.f, 2.f },
			kd::make_node(
				std::vector{ 5.f, 4.f },
				kd::make_node(std::vector{ 2.f, 3.f }),
				kd::make_node(std::vector{ 4.f, 7.f })
			),
			kd::make_node(
				std::vector{ 9.f, 6.f },
				kd::make_node(std::vector{ 8.f, 1.f }),
				kd::make_leaf<std::vector<float>>()
			)
		),
		2
	);

	const auto actual_tree{ kd::tree_t<std::vector<float>>::build(points, 2) };

	EXPECT_EQ(expected_tree, actual_tree);
}

TEST(tree, find_nearest) {
	const std::vector<kd::float2> points{
		{2, 3},
		{5, 4},
		{9, 6},
		{4, 7},
		{8, 1},
		{7, 2},
	};

	const auto tree{ kd::build_tree<kd::float2>(points) };

	EXPECT_EQ(tree.find_nearest({ 8, 7 }).value(), (kd::float2{ 9, 6 }));
	EXPECT_EQ(tree.find_nearest({ 2, 3 }).value(), (kd::float2{ 2, 3 }));
	EXPECT_EQ(tree.find_nearest({ 5, 6 }).value(), (kd::float2{ 4, 7 }));
	EXPECT_EQ(tree.find_nearest({ 0, 0 }).value(), (kd::float2{ 2, 3 }));
}