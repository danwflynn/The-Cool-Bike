#pragma once

#include "math/Transform.h"
#include <vector>

class WheelHitbox {
public:
    WheelHitbox(const std::vector<glm::vec3>& vertices, Transform& parentTransform);

    glm::vec3 GetWorldCenter() const;
    float GetWorldThickness() const;

    float WheelHitbox::GetWorldVerticalRadius() const;
    bool WheelHitbox::IsCollidingWithGround() const;
    float WheelHitbox::GetGroundPenetration() const;

private:
    Transform& transform;

    glm::vec3 localCenter;
    float radius;
    float thickness;
};
