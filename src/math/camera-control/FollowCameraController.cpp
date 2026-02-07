#include "FollowCameraController.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
    glm::vec3 DirectionToEuler(const glm::vec3& dir)
    {
        float pitch = asin(glm::clamp(dir.y, -1.0f, 1.0f));
        float yaw   = atan2(dir.x, -dir.z);
        return { pitch, yaw, 0.0f };
    }
}

FollowCameraController::FollowCameraController(const Bike& targetBike)
    : targetBike(targetBike)
{
}

void FollowCameraController::Update(Camera& camera, float dt) {
    const Transform& bikeTransform = targetBike.getTransform();
    glm::vec3 bikePos = bikeTransform.GetPosition();
    
    glm::vec3 bikeForward = bikeTransform.GetRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 flatForward = glm::normalize(glm::vec3(bikeForward.x, 0.0f, bikeForward.z));

    glm::vec3 targetPos = bikePos - (flatForward * followDistance) + (glm::vec3(0.0f, 1.0f, 0.0f) * height);

    glm::vec3 currentPos = camera.GetPosition();
    float blend = 1.0f - glm::exp(-smoothing * dt);
    glm::vec3 newPos;
    newPos.x = glm::mix(currentPos.x, targetPos.x, blend);
    newPos.z = glm::mix(currentPos.z, targetPos.z, blend);
    newPos.y = targetPos.y;

    camera.SetPosition(newPos);

    glm::vec3 lookTarget = bikePos + glm::vec3(0.0f, 0.8f, 0.0f);
    glm::vec3 lookDir = glm::normalize(lookTarget - newPos);

    glm::vec3 euler = DirectionToEuler(lookDir);
    euler.x -= pitchOffset;

    camera.SetRotation(euler);
}
