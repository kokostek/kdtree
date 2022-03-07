#include <gtest/gtest.h>
#include "kdtree/node.hpp"
#include "kdtree/float2.hpp"

namespace kd = kdtree;

TEST(node, basics) {
	kd::node<kd::float2> node{ {1, 2} };
	EXPECT_NO_FATAL_FAILURE(node.value());
}

TEST(node, equals_simple) {
	auto node1 = kd::make_node(kd::float2{ 1, 2 });
	auto node2 = kd::make_node(kd::float2{ 3, 4 });
	auto node3 = kd::make_node(kd::float2{ 1, 2 });

	EXPECT_TRUE(*node1 == *node1);
	EXPECT_TRUE(*node1 != *node2);
	EXPECT_TRUE(*node1 == *node3);
}

TEST(node, equals_complex) {

	auto tree1 = kd::node(
		kd::float2{ 0, 0 },
		kd::make_node(
			kd::float2{ 1, 2 },
			kd::make_node(kd::float2{ 3, 4 }),
			kd::make_node(kd::float2{ 5, 6 })),
		kd::make_node<kd::float2>(
			kd::float2{ 7, 8 },
			kd::make_node(kd::float2{ 9, 10 }),
			kd::make_node(kd::float2{ 11, 12 })));
	
	auto tree2 = kd::node(
		kd::float2{ 0, 0 },
		kd::make_node(
			kd::float2{ 1, 2 },
			kd::make_node(kd::float2{ 3, 4 }),
			kd::make_node(kd::float2{ 5, 6 })),
		kd::make_node(
			kd::float2{ 7, 8 },
			kd::make_node(kd::float2{ 9, 10 }),
			kd::make_node(kd::float2{ 11, 12 })));

	auto tree3 = kd::node(
		kd::float2{ 0, 0 },
		kd::make_node(
			kd::float2{ 1, 2 },
			kd::make_node(kd::float2{ 3, 4 }),
			kd::make_leaf<kd::float2>()),
		kd::make_leaf<kd::float2>());

	EXPECT_TRUE(tree1 == tree1);
	EXPECT_TRUE(tree1 == tree2);
	EXPECT_TRUE(tree1 != tree3);

	EXPECT_TRUE(tree2 == tree1);
	EXPECT_TRUE(tree2 == tree2);
	EXPECT_TRUE(tree2 != tree3);

	EXPECT_TRUE(tree3 != tree1);
	EXPECT_TRUE(tree3 != tree2);
	EXPECT_TRUE(tree3 == tree3);
}