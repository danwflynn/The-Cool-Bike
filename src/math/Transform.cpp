#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
    : m_Position(0.0f),
      m_Rotation(0.0f),
      m_Scale(1.0f) {
}

void Transform::SetPosition(const glm::vec3& position) {
    m_Position = position;
}

const glm::vec3& Transform::GetPosition() const {
    return m_Position;
}

void Transform::SetRotation(const glm::vec3& rotation) {
    m_Rotation = rotation;
}

const glm::vec3& Transform::GetRotation() const {
    return m_Rotation;
}

void Transform::SetScale(const glm::vec3& scale) {
    m_Scale = scale;
}

const glm::vec3& Transform::GetScale() const {
    return m_Scale;
}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 model(1.0f);

    model = glm::translate(model, m_Position);

    model = glm::rotate(model, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, m_Scale);

    return model;
}
