#include "Camera.h"

Camera::Camera(float fov, float aspect, float nearPlane, float farPlane)
    : m_Position(0.0f), m_Rotation(0.0f) 
{
    // Projection is fixed at construction
    m_Projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

glm::mat4 Camera::GetViewMatrix() const {
    glm::mat4 view(1.0f);

    // Apply rotation (pitch, yaw, roll)
    view = glm::rotate(view, m_Rotation.x, glm::vec3(1, 0, 0));
    view = glm::rotate(view, m_Rotation.y, glm::vec3(0, 1, 0));
    view = glm::rotate(view, m_Rotation.z, glm::vec3(0, 0, 1));

    // Apply translation (inverse of camera position)
    view = glm::translate(view, -m_Position);

    return view;
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return m_Projection;
}

void Camera::SetPosition(const glm::vec3& position) {
    m_Position = position;
}

void Camera::SetRotation(const glm::vec3& rotation) {
    m_Rotation = rotation;
}
