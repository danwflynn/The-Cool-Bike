#include <gtest/gtest.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>

#include "math/Transform.h"

constexpr float EPS = 1e-5f;

TEST(TransformTests, DefaultValuesAreCorrect) {
    Transform t;

    EXPECT_TRUE(glm::all(glm::epsilonEqual(t.GetPosition(), glm::vec3(0.0f), EPS)));
    EXPECT_TRUE(glm::all(glm::epsilonEqual(t.GetRotationEuler(), glm::vec3(0.0f), EPS)));
    EXPECT_TRUE(glm::all(glm::epsilonEqual(t.GetScale(), glm::vec3(1.0f), EPS)));
}

TEST(TransformTests, PositionIsStoredCorrectly) {
    Transform t;
    t.SetPosition({1.0f, 2.0f, 3.0f});

    EXPECT_TRUE(glm::all(glm::epsilonEqual(t.GetPosition(), glm::vec3(1.0f, 2.0f, 3.0f), EPS)));
}

TEST(TransformTests, RotationIsStoredCorrectly) {
    Transform t;
    t.SetRotationEuler({0.1f, 0.2f, 0.3f});

    EXPECT_TRUE(glm::all(glm::epsilonEqual(t.GetRotationEuler(), glm::vec3(0.1f, 0.2f, 0.3f), EPS)));
}

TEST(TransformTests, ScaleIsStoredCorrectly) {
    Transform t;
    t.SetScale({2.0f, 3.0f, 4.0f});

    EXPECT_TRUE(glm::all(glm::epsilonEqual(t.GetScale(), glm::vec3(2.0f, 3.0f, 4.0f), EPS)));
}

TEST(TransformTests, TranslationMatrixIsCorrect) {
    Transform t;
    t.SetPosition({1.0f, 2.0f, 3.0f});

    glm::mat4 m = t.GetMatrix();

    EXPECT_NEAR(m[3][0], 1.0f, EPS);
    EXPECT_NEAR(m[3][1], 2.0f, EPS);
    EXPECT_NEAR(m[3][2], 3.0f, EPS);
}

TEST(TransformTests, ScaleMatrixIsCorrect) {
    Transform t;
    t.SetScale({2.0f, 3.0f, 4.0f});

    glm::mat4 m = t.GetMatrix();

    EXPECT_NEAR(m[0][0], 2.0f, EPS);
    EXPECT_NEAR(m[1][1], 3.0f, EPS);
    EXPECT_NEAR(m[2][2], 4.0f, EPS);
}

TEST(TransformTests, RotationAroundXIsCorrect) {
    Transform t;
    t.SetRotationEuler({glm::half_pi<float>(), 0.0f, 0.0f});

    glm::mat4 m = t.GetMatrix();

    // Rotating 90 deg around X swaps Y/Z axes
    EXPECT_NEAR(m[1][1], 0.0f, EPS);
    EXPECT_NEAR(m[2][1], -1.0f, EPS);
    EXPECT_NEAR(m[1][2], 1.0f, EPS);
    EXPECT_NEAR(m[2][2], 0.0f, EPS);
}

TEST(TransformTests, RotationAroundYIsCorrect) {
    Transform t;
    t.SetRotationEuler({0.0f, glm::half_pi<float>(), 0.0f});

    glm::mat4 m = t.GetMatrix();

    EXPECT_NEAR(m[0][0], 0.0f, EPS);
    EXPECT_NEAR(m[2][0], 1.0f, EPS);
    EXPECT_NEAR(m[0][2], -1.0f, EPS);
    EXPECT_NEAR(m[2][2], 0.0f, EPS);
}

TEST(TransformTests, RotationAroundZIsCorrect) {
    Transform t;
    t.SetRotationEuler({0.0f, 0.0f, glm::half_pi<float>()});

    glm::mat4 m = t.GetMatrix();

    EXPECT_NEAR(m[0][0], 0.0f, EPS);
    EXPECT_NEAR(m[1][0], -1.0f, EPS);
    EXPECT_NEAR(m[0][1], 1.0f, EPS);
    EXPECT_NEAR(m[1][1], 0.0f, EPS);
}

TEST(TransformTests, TransformOrderIsTranslateRotateScale) {
    Transform t;
    t.SetPosition({1.0f, 0.0f, 0.0f});
    t.SetRotationEuler({0.0f, 0.0f, 0.0f});
    t.SetScale({2.0f, 2.0f, 2.0f});

    glm::mat4 m = t.GetMatrix();

    // Translation should remain unaffected by scale
    EXPECT_NEAR(m[3][0], 1.0f, EPS);
    EXPECT_NEAR(m[3][1], 0.0f, EPS);
    EXPECT_NEAR(m[3][2], 0.0f, EPS);

    // Scale matrix should be correct
    EXPECT_NEAR(m[0][0], 2.0f, EPS);
    EXPECT_NEAR(m[1][1], 2.0f, EPS);
    EXPECT_NEAR(m[2][2], 2.0f, EPS);
}
