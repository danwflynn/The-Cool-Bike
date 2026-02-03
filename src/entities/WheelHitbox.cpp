#include "WheelHitbox.h"
#include <limits>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

WheelHitbox::WheelHitbox(const std::vector<glm::vec3>& vertices, Transform& parentTransform)
    : transform(parentTransform),
      localCenter(0.0f),
      radius(0.0f),
      thickness(0.0f)
{
    // Compute AABB in local space
    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());

    for (const glm::vec3& v : vertices) {
        min.x = std::min(min.x, v.x);
        min.y = std::min(min.y, v.y);
        min.z = std::min(min.z, v.z);

        max.x = std::max(max.x, v.x);
        max.y = std::max(max.y, v.y);
        max.z = std::max(max.z, v.z);
    }

    // Center of wheel in local (bike) space
    localCenter = (min + max) * 0.5f;

    // Wheel assumed upright
    radius    = (max.y - min.y) * 0.5f;
    thickness = (max.x - min.x) * 0.5f;
}

glm::vec3 WheelHitbox::GetWorldCenter() const {
    glm::mat4 world = transform.GetMatrix();
    glm::vec4 local(localCenter, 1.0f);
    return glm::vec3(world * local);
}

float WheelHitbox::GetWorldThickness() const {
    glm::mat4 world = transform.GetMatrix();

    glm::vec3 localThicknessVec(thickness, 0.0f, 0.0f);
    glm::vec3 worldThicknessVec = glm::vec3(world * glm::vec4(localThicknessVec, 0.0f));

    return glm::length(worldThicknessVec);
}

glm::vec3 WheelHitbox::GetWorldNormal() const {
    glm::mat4 world = transform.GetMatrix();
    return glm::normalize(glm::vec3(world * glm::vec4(0, 0, 1, 0)));
}

float WheelHitbox::GetLowestPointY() const {
    glm::vec3 center = GetWorldCenter();
    float verticalRadius = radius * transform.GetScale().y; // full radius
    return center.y - verticalRadius;
}

bool WheelHitbox::IsCollidingWithGround() const {
    return GetLowestPointY() <= 0.0f;
}

float WheelHitbox::GetGroundPenetration() const {
    return std::max(0.0f, -GetLowestPointY());
}
