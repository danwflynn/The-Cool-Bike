#pragma once

#include "math/Transform.h"
#include <vector>

class WheelHitbox {
public:
    WheelHitbox(const std::vector<glm::vec3>& vertices, Transform& parentTransform);

    glm::vec3 GetWorldCenter() const;
    float GetWorldRadius() const;
    float GetWorldThickness() const;

private:
    Transform& transform;

    glm::vec3 localCenter;
    float radius;
    float thickness;
};
