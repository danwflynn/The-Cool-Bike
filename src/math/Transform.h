#pragma once

#include <glm/glm.hpp>

class Transform {
public:
    Transform();

    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const;

    // Rotation (Euler angles, radians)
    void SetRotation(const glm::vec3& rotation);
    const glm::vec3& GetRotation() const;

    void SetScale(const glm::vec3& scale);
    const glm::vec3& GetScale() const;

    glm::mat4 GetMatrix() const;

private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
};
