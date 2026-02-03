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

    Bike(Camera& cameraRef);

    Transform& getTransform();
    bool IsCollidingWithGround() const;
    void applyVerticalForce();

private:
    Transform transform;
    Camera& camera;

    std::optional<WheelHitbox> frontWheel;
    std::optional<WheelHitbox> backWheel;

    float vertical_velocity;
};
