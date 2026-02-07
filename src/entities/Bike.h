#pragma once
#include "math/Transform.h"
#include "math/Camera.h"
#include "WheelHitbox.h"
#include <optional>


class Bike {
public:
    static constexpr float SCALE = 0.01f;
    static constexpr float GRAVITATIONAL_ACCELERATION = 0.00001;
    static constexpr float MAX_FALL_SPEED = -0.015;
    static constexpr float ANGULAR_ACCELERATION = 0.00001f;
    static constexpr float MAX_ANGULAR_SPEED    = 0.05f;

    Bike(Camera& cameraRef);

    const Transform& getTransform() const;
    void RotateAroundPoint(const glm::vec3& point, const glm::vec3& axis, float angle);
    void applyVerticalForce();

private:
    Transform transform;
    Camera& camera;

    std::optional<WheelHitbox> frontWheel;
    std::optional<WheelHitbox> backWheel;

    float vertical_velocity;
    float angular_velocity;
};
