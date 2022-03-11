#include <gtest/gtest.h>
#include <kdtree/point2d.hpp>
#include <cstdio>

namespace kd = kdtree;

TEST(float2, access_by_index) {
    EXPECT_EQ((kd::float2{ 1.f, 2.f })[0] , 1.f);
    EXPECT_EQ((kd::float2{ 1.f, 2.f })[1] , 2.f);
}