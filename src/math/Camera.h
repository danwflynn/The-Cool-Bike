#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov, float aspect, float nearPlane, float farPlane);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec3& GetRotation() const { return m_Rotation; }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;

    glm::mat4 m_Projection;
};
