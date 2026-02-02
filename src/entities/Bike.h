#pragma once
#include "math/Transform.h"
#include "math/Camera.h"
#include "WheelHitbox.h"
#include <optional>


class Bike {
public:
    static constexpr float SCALE = 0.01f;

    Bike(Camera& cameraRef);

    Transform& getTransform();
    bool Bike::IsCollidingWithGround() const;

private:
    Transform transform;
    Camera& camera;

    std::optional<WheelHitbox> frontWheel;
    std::optional<WheelHitbox> backWheel;
};
