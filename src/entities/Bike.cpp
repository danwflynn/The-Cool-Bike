#include "Bike.h"

Bike::Bike(Camera& cameraRef, float bottomY) : camera(cameraRef)
{
    transform.SetScale(glm::vec3(SCALE));
    transform.SetPosition(glm::vec3(0.0f, -1 * bottomY * SCALE, -2.0f));
    transform.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
}

Transform& Bike::getTransform() {
    return transform;
}