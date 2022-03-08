#include <gtest/gtest.h>
#include <kdtree/float2.hpp>

namespace kd = kdtree;

TEST(float2, access_by_field) {
	EXPECT_EQ(kd::float2(1.f, 2.f).x, 1.f);
	EXPECT_EQ(kd::float2(1.f, 2.f).y, 2.f);
}

TEST(float2, access_by_index) {
	EXPECT_EQ(kd::float2(1.f, 2.f)[0], 1.f);
	EXPECT_EQ(kd::float2(1.f, 2.f)[1], 2.f);
}