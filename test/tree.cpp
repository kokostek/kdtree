#include <kdtree/tree.hpp>
#include <kdtree/point2d.hpp>
#include <kdtree/point3d.hpp>
#include <gtest/gtest.h>
#include <vector>

namespace kd = kdtree;

TEST(tree, empty) {
    kd::tree<kd::float2> tree1;
    EXPECT_FALSE(tree1.root());
}

TEST(tree, single_node) {
    const auto tree = kd::make_tree<kd::float2>(kd::make_node(kd::float2{ 1, 2 }));
    EXPECT_TRUE(tree.root());
    EXPECT_EQ(tree.root()->value(), (kd::float2{ 1, 2 }));
    EXPECT_FALSE(tree.root()->left());
    EXPECT_FALSE(tree.root()->right());
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

    const kd::tree<kd::float2> tree4;

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

TEST(tree, build_1) {
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

    const auto actual_tree{ kd::build_tree(points) };

    EXPECT_EQ(expected_tree, actual_tree);
}

TEST(tree, build_2) {
    const std::vector<std::vector<float>> points{
        {2, 3},
        {5, 4},
        {9, 6},
        {4, 7},
        {8, 1},
        {7, 2},
    };

    const auto expected_tree = kd::tree<std::vector<float>>(
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

    const auto actual_tree{ kd::tree<std::vector<float>>::build(points, 2) };

    EXPECT_EQ(expected_tree, actual_tree);
}

TEST(tree, find_nearest_1) {

    const auto tree{ kd::make_tree<kd::float2>() };

    EXPECT_NO_THROW(tree.find_nearest(kd::float2{}));

    EXPECT_EQ(tree.find_nearest(kd::float2{}), (kd::float2{}));
    EXPECT_EQ(tree.find_nearest(kd::float2{ 1, 2 }), (kd::float2{}));
    EXPECT_EQ(tree.find_nearest(kd::float2{ 3, 4 }), (kd::float2{}));
}

TEST(tree, find_nearest_2) {
    const auto tree{ kd::tree<std::vector<float>>(33) };

    EXPECT_NO_THROW(tree.find_nearest(std::vector<float>{}));

    EXPECT_EQ(tree.find_nearest(std::vector<float>{}).size(), 0);
    EXPECT_EQ(tree.find_nearest(std::vector<float>(1)).size(), 0);
    EXPECT_EQ(tree.find_nearest(std::vector<float>(2)).size(), 0);
    EXPECT_EQ(tree.find_nearest(std::vector<float>(33)).size(), 0);
}

TEST(tree, find_nearest_3) {
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

TEST(tree, find_nearest_4) {
    const std::vector<kd::float2> points{
        {2, 3},
        {5, 4},
        {9, 6},
        {4, 7},
        {8, 1},
        {7, 2},
    };

    const auto tree{ kd::build_tree(points) };

    EXPECT_EQ(tree.find_nearest({ 8, 7 }), (kd::float2{ 9, 6 }));
    EXPECT_EQ(tree.find_nearest({ 2, 3 }), (kd::float2{ 2, 3 }));
    EXPECT_EQ(tree.find_nearest({ 5, 6 }), (kd::float2{ 4, 7 }));
    EXPECT_EQ(tree.find_nearest({ 0, 0 }), (kd::float2{ 2, 3 }));
}

TEST(tree, find_nearest_n_1) {

    const auto tree{ kd::make_tree<kd::float2>() };

    EXPECT_NO_THROW(tree.find_nearest_n(kd::float2{}, 3));
    EXPECT_EQ(tree.find_nearest_n(kd::float2{}, 3).size(), 0);
}

TEST(tree, find_nearest_n_2) {
    const auto tree{
        kd::build_tree({
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
        })
    };

    const std::vector expected_results{
        kd::float2{0.86f, -0.41f},
        kd::float2{1.26f, -0.44f},
        kd::float2{0.49f, -0.12f}, 
    };

    const auto actual_results{ tree.find_nearest_n({0.8f, -0.6f}, 3) };

    EXPECT_EQ(expected_results, actual_results);
}

TEST(tree, find_nearest_n_3) {
    const auto tree{
        kd::build_tree({
            kd::float2{-0.75f, 1.32f},
            kd::float2{1.25f, -1.6f},
            kd::float2{-1.47f, -1.72f},
        })
    };

    const std::vector expected_results{
        kd::float2{1.25f, -1.6f},
        kd::float2{-0.75f, 1.32f},
        kd::float2{-1.47f, -1.72f},
    };

    const auto actual_results{ tree.find_nearest_n({0.8f, -0.6f}, 5) };

    EXPECT_EQ(expected_results, actual_results);
}

TEST(tree, find_nearest_n_4) {
    const auto tree{
        kd::build_tree({
            kd::float3{-0.75f, 1.32f, 1.25f},
            kd::float3{-1.6f, -1.47f, -1.72f},
            kd::float3{1.86f, 0.09f, -0.05f},
            kd::float3{0.56f, -0.96f, -0.18f},
            kd::float3{-1.18f, 0.61f, -0.95f},
            kd::float3{0.36f, 1.06f, -0.12f},
            kd::float3{0.83f, -1.21f, -1.19f},
            kd::float3{0.22f, -0.21f, -1.41f},
            kd::float3{-0.48f, 1.2f, -0.71f},
            kd::float3{0.03f, -0.39f, 0.19f},
            kd::float3{0.35f, 0.59f, 0.52f},
            kd::float3{0.83f, 0.43f, 0.2f},
            kd::float3{-0.51f, -3.07f, -0.4f},
            kd::float3{-0.8f, -0.7f, -1.74f},
            kd::float3{0.75f, -1.86f, 0.42f},
            kd::float3{-0.24f, 0.43f, 0.87f},
            kd::float3{2.01f, 1.37f, 1.06f},
        })
    };

    const std::vector expected_results{
        kd::float3{0.56f, -0.96f, -0.18f},
        kd::float3{0.03f, -0.39f, 0.19f},
        kd::float3{0.83f, 0.43f, 0.2f},
        kd::float3{1.86f, 0.09f, -0.05f},
    };

    const auto actual_results{ tree.find_nearest_n({0.8f, -0.6f, 0.1f}, 4) };

    EXPECT_EQ(expected_results, actual_results);
}