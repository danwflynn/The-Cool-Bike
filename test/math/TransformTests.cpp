#include <gtest/gtest.h>
#include <glm/gtc/matrix_transform.hpp>

#include "math/Transform.h"


// Basic state tests
TEST(TransformTests, DefaultValuesAreCorrect) {
    Transform t;

    EXPECT_EQ(t.GetPosition(), glm::vec3(0.0f));
    EXPECT_EQ(t.GetRotation(), glm::vec3(0.0f));
    EXPECT_EQ(t.GetScale(),    glm::vec3(1.0f));
}

TEST(TransformTests, PositionIsStoredCorrectly) {
    Transform t;
    t.SetPosition({1.0f, 2.0f, 3.0f});

    auto pos = t.GetPosition();
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 2.0f);
    EXPECT_FLOAT_EQ(pos.z, 3.0f);
}

TEST(TransformTests, RotationIsStoredCorrectly) {
    Transform t;
    t.SetRotation({0.1f, 0.2f, 0.3f});

    auto rot = t.GetRotation();
    EXPECT_FLOAT_EQ(rot.x, 0.1f);
    EXPECT_FLOAT_EQ(rot.y, 0.2f);
    EXPECT_FLOAT_EQ(rot.z, 0.3f);
}

TEST(TransformTests, ScaleIsStoredCorrectly) {
    Transform t;
    t.SetScale({2.0f, 3.0f, 4.0f});

    auto scale = t.GetScale();
    EXPECT_FLOAT_EQ(scale.x, 2.0f);
    EXPECT_FLOAT_EQ(scale.y, 3.0f);
    EXPECT_FLOAT_EQ(scale.z, 4.0f);
}

// Matrix math tests
TEST(TransformTests, TranslationMatrixIsCorrect) {
    Transform t;
    t.SetPosition({1.0f, 2.0f, 3.0f});

    glm::mat4 m = t.GetMatrix();

    EXPECT_FLOAT_EQ(m[3][0], 1.0f);
    EXPECT_FLOAT_EQ(m[3][1], 2.0f);
    EXPECT_FLOAT_EQ(m[3][2], 3.0f);
}

TEST(TransformTests, ScaleMatrixIsCorrect) {
    Transform t;
    t.SetScale({2.0f, 3.0f, 4.0f});

    glm::mat4 m = t.GetMatrix();

    EXPECT_FLOAT_EQ(m[0][0], 2.0f);
    EXPECT_FLOAT_EQ(m[1][1], 3.0f);
    EXPECT_FLOAT_EQ(m[2][2], 4.0f);
}

TEST(TransformTests, RotationAroundXIsCorrect) {
    Transform t;
    t.SetRotation({glm::half_pi<float>(), 0.0f, 0.0f});

    glm::mat4 m = t.GetMatrix();

    // Rotating 90 degrees around X swaps Y/Z axes
    EXPECT_NEAR(m[1][1], 0.0f, 0.0001f);
    EXPECT_NEAR(m[2][1], -1.0f, 0.0001f);
    EXPECT_NEAR(m[1][2], 1.0f, 0.0001f);
    EXPECT_NEAR(m[2][2], 0.0f, 0.0001f);
}

TEST(TransformTests, RotationAroundYIsCorrect) {
    Transform t;
    t.SetRotation({0.0f, glm::half_pi<float>(), 0.0f});

    glm::mat4 m = t.GetMatrix();

    EXPECT_NEAR(m[0][0], 0.0f, 0.0001f);
    EXPECT_NEAR(m[2][0], 1.0f, 0.0001f);
    EXPECT_NEAR(m[0][2], -1.0f, 0.0001f);
    EXPECT_NEAR(m[2][2], 0.0f, 0.0001f);
}

TEST(TransformTests, RotationAroundZIsCorrect) {
    Transform t;
    t.SetRotation({0.0f, 0.0f, glm::half_pi<float>()});

    glm::mat4 m = t.GetMatrix();

    EXPECT_NEAR(m[0][0], 0.0f, 0.0001f);
    EXPECT_NEAR(m[1][0], -1.0f, 0.0001f);
    EXPECT_NEAR(m[0][1], 1.0f, 0.0001f);
    EXPECT_NEAR(m[1][1], 0.0f, 0.0001f);
}

// Combined transform order
TEST(TransformTests, TransformOrderIsTranslateRotateScale) {
    Transform t;
    t.SetPosition({1.0f, 0.0f, 0.0f});
    t.SetRotation({0.0f, 0.0f, 0.0f});
    t.SetScale({2.0f, 2.0f, 2.0f});

    glm::mat4 m = t.GetMatrix();

    // Translation should NOT be scaled
    EXPECT_FLOAT_EQ(m[3][0], 1.0f);
    EXPECT_FLOAT_EQ(m[3][1], 0.0f);
    EXPECT_FLOAT_EQ(m[3][2], 0.0f);
}
