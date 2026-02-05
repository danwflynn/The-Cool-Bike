#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform {
public:
    Transform();

    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const;

    void SetRotation(const glm::quat& rotation);
    const glm::quat& GetRotation() const;

    void SetRotationEuler(const glm::vec3& eulerRadians);
    glm::vec3 GetRotationEuler() const;

    void RotateAroundAxis(const glm::vec3& axis, float angleRadians);

    void SetScale(const glm::vec3& scale);
    const glm::vec3& GetScale() const;

    glm::mat4 GetMatrix() const;

private:
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
};
