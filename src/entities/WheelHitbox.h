#pragma once

#include "math/Transform.h"
#include <vector>

class WheelHitbox {
public:
    static constexpr float THICKNESS_DAMPENER = 0.5f;

    WheelHitbox(const std::vector<glm::vec3>& vertices, Transform& parentTransform);

    glm::vec3 GetWorldCenter() const;
    float GetWorldThickness() const;

    glm::vec3 GetWheelAxisWorld() const;
    glm::vec3 GetLowestPoint() const;
    bool IsCollidingWithGround() const;
    float GetGroundPenetration() const;;

private:
    Transform& transform;

    glm::vec3 localCenter;
    float radius;
    float thickness;
};
