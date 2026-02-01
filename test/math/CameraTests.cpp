#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "math/Camera.h"

// Small epsilon for float comparisons
static constexpr float EPSILON = 1e-5f;

TEST(CameraTests, ProjectionMatrixIsCreated) {
    Camera cam(90.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    glm::mat4 proj = cam.GetProjectionMatrix();

    // Projection matrix should NOT be identity
    EXPECT_NE(proj, glm::mat4(1.0f));

    // These should always be true for perspective projection
    EXPECT_NE(proj[0][0], 0.0f);
    EXPECT_NE(proj[1][1], 0.0f);
    EXPECT_NE(proj[2][2], 0.0f);
    EXPECT_EQ(proj[3][3], 0.0f);
}

TEST(CameraTests, ViewMatrixIsIdentityAtOrigin) {
    Camera cam(60.0f, 1.0f, 0.1f, 100.0f);

    glm::mat4 view = cam.GetViewMatrix();
    glm::mat4 identity(1.0f);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR(view[i][j], identity[i][j], EPSILON);
        }
    }
}

TEST(CameraTests, ViewMatrixAppliesTranslationCorrectly) {
    Camera cam(60.0f, 1.0f, 0.1f, 100.0f);
    cam.SetPosition({ 1.0f, 2.0f, 3.0f });

    glm::mat4 view = cam.GetViewMatrix();

    // Translation is inverse of position
    EXPECT_NEAR(view[3][0], -1.0f, EPSILON);
    EXPECT_NEAR(view[3][1], -2.0f, EPSILON);
    EXPECT_NEAR(view[3][2], -3.0f, EPSILON);
}

TEST(CameraTests, ViewMatrixAppliesRotationAroundX) {
    Camera cam(60.0f, 1.0f, 0.1f, 100.0f);
    cam.SetRotation({ glm::radians(90.0f), 0.0f, 0.0f });

    glm::mat4 view = cam.GetViewMatrix();

    // Rotating 90 degrees around X swaps Y/Z axes
    EXPECT_NEAR(view[1][1], 0.0f, EPSILON);
    EXPECT_NEAR(view[1][2], 1.0f, EPSILON);
    EXPECT_NEAR(view[2][1], -1.0f, EPSILON);
    EXPECT_NEAR(view[2][2], 0.0f, EPSILON);
}

TEST(CameraTests, ViewMatrixAppliesRotationAroundY) {
    Camera cam(60.0f, 1.0f, 0.1f, 100.0f);
    cam.SetRotation({ 0.0f, glm::radians(90.0f), 0.0f });

    glm::mat4 view = cam.GetViewMatrix();

    EXPECT_NEAR(view[0][0], 0.0f, EPSILON);
    EXPECT_NEAR(view[0][2], -1.0f, EPSILON);
    EXPECT_NEAR(view[2][0], 1.0f, EPSILON);
    EXPECT_NEAR(view[2][2], 0.0f, EPSILON);
}

TEST(CameraTests, ProjectionMatrixDoesNotChangeAfterMovement) {
    Camera cam(60.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 originalProjection = cam.GetProjectionMatrix();

    cam.SetPosition({ 5.0f, 5.0f, 5.0f });
    cam.SetRotation({ 1.0f, 1.0f, 1.0f });

    glm::mat4 newProjection = cam.GetProjectionMatrix();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR(originalProjection[i][j], newProjection[i][j], EPSILON);
        }
    }
}
