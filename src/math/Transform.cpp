#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform()
    : m_Position(0.0f), m_Rotation(glm::quat(1, 0, 0, 0)), m_Scale(1.0f) {
}

void Transform::SetPosition(const glm::vec3& position) {
    m_Position = position;
}

const glm::vec3& Transform::GetPosition() const {
    return m_Position;
}

void Transform::SetRotation(const glm::quat& rotation) {
    m_Rotation = rotation;
}

const glm::quat& Transform::GetRotation() const {
    return m_Rotation;
}

void Transform::SetRotationEuler(const glm::vec3& eulerRadians) {
    m_Rotation = glm::quat(eulerRadians);
}

glm::vec3 Transform::GetRotationEuler() const {
    return glm::eulerAngles(m_Rotation);
}

void Transform::RotateAroundAxis(const glm::vec3& axis, float angleRadians) {
    glm::quat q = glm::angleAxis(angleRadians, glm::normalize(axis));
    m_Rotation = q * m_Rotation;
}

void Transform::SetScale(const glm::vec3& scale) {
    m_Scale = scale;
}

const glm::vec3& Transform::GetScale() const {
    return m_Scale;
}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 r = glm::toMat4(m_Rotation);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), m_Scale);
    return t * r * s;
}
