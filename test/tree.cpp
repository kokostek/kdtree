#include <gtest/gtest.h>
#include <kdtree/tree.hpp>
#include <kdtree/float2.hpp>

namespace kd = kdtree;

TEST(tree, empty) {
	kd::tree<kd::float2, float, 2> tree;
	EXPECT_FALSE(tree.root());
}

TEST(tree, single_node) {
	kd::tree<kd::float2, float, 2> tree(kd::make_node(kd::float2{1, 2}));
	EXPECT_TRUE(tree.root());
	EXPECT_EQ(tree.root()->value(), kd::float2(1, 2));
	EXPECT_FALSE(tree.root()->left(), kd::float2(1, 2));
	EXPECT_FALSE(tree.root()->right(), kd::float2(1, 2));
}

TEST(tree, find_nearest_no_throw) {
	
	kd::tree<kd::float2, float, 2> tree(
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

	EXPECT_NO_THROW(tree.find_nearest(kd::float2(1.5f, 0.5f)));
}

TEST(tree, equality) {
	const kd::tree<kd::float2, float, 2> tree1(
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

	const kd::tree<kd::float2, float, 2> tree2(
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

	const kd::tree<kd::float2, float, 2> tree3(
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

	const kd::tree<kd::float2, float, 2> tree4;

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

	const kd::tree<kd::float2, float, 2> expected_tree(
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

	const auto actual_tree{ kd::tree<kd::float2, float, 2>::build(points) };

	EXPECT_EQ(expected_tree, actual_tree);
}