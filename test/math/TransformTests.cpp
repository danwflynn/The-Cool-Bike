#include <gtest/gtest.h>
#include "math/Transform.h"

TEST(TransformTests, PositionIsStoredCorrectly) {
    Transform t;
    t.SetPosition({1.0f, 2.0f, 3.0f});

    auto pos = t.GetPosition();
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 2.0f);
    EXPECT_FLOAT_EQ(pos.z, 3.0f);
}
