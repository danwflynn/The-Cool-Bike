#include "Bike.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Bike::Bike(Camera& cameraRef) : camera(cameraRef), vertical_velocity(0.0f), angular_velocity(0.0f) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        "assets/bike/standard_bike.glb",
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality
    );
    if (!scene || !scene->mRootNode || scene->mNumMeshes == 0) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        if (mesh->mName == aiString("polygon2")) {
            std::vector<glm::vec3> posVertices;
            std::vector<glm::vec3> negVertices;
            posVertices.reserve(mesh->mNumVertices);
            negVertices.reserve(mesh->mNumVertices);
            for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                if (mesh->mVertices[v].z >= 0) posVertices.push_back(glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z));
                else negVertices.push_back(glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z));
            }
            frontWheel.emplace(posVertices, transform);
            backWheel.emplace(negVertices, transform);
        }
    }

    transform.SetScale(glm::vec3(SCALE));
    transform.SetPosition(glm::vec3(0.0f, 20.0f, -2.0f));
    transform.SetRotation(glm::vec3(glm::radians(-35.0f), 0.0f, glm::radians(0.0f)));
}

Transform& Bike::getTransform() {
    return transform;
}

static glm::vec3 GetLocalXAxisWorld(const Transform& transform) {
    glm::mat4 world = transform.GetMatrix();
    return glm::normalize(glm::vec3(world * glm::vec4(1, 0, 0, 0)));
}

void Bike::RotateAroundPoint(const glm::vec3& point, const glm::vec3& axis, float angle) {
    glm::vec3 pos = transform.GetPosition();

    pos -= point;

    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, axis);
    pos = glm::vec3(rot * glm::vec4(pos, 1.0f));

    pos += point;

    transform.SetPosition(pos);

    glm::vec3 rotEuler = transform.GetRotation();
    rotEuler += axis * angle;
    transform.SetRotation(rotEuler);
}

void Bike::applyVerticalForce() {
    bool backGrounded = backWheel && backWheel->IsCollidingWithGround();
    bool frontGrounded = frontWheel && frontWheel->IsCollidingWithGround();
    bool singleWheelGrounded = backGrounded ^ frontGrounded;

    if (!backGrounded && !frontGrounded) {
        vertical_velocity -= GRAVITATIONAL_ACCELERATION;
        vertical_velocity = std::max(vertical_velocity, MAX_FALL_SPEED);
    } else {
        if (singleWheelGrounded) {
            WheelHitbox* pivotWheel = backGrounded ? backWheel.operator->() : frontWheel.operator->();
            WheelHitbox* freeWheel = backGrounded ? frontWheel.operator->() : backWheel.operator->();

            glm::vec3 pivotPoint = pivotWheel->GetLowestPoint();
            glm::vec3 axis = GetLocalXAxisWorld(transform);

            glm::vec3 toFree = freeWheel->GetWorldCenter() - pivotWheel->GetWorldCenter();
            glm::vec3 torqueDir = glm::cross(axis, toFree);
            float rotationSign = (glm::dot(torqueDir, glm::vec3(0, -1, 0)) > 0.0f) ? 1.0f : -1.0f;

            angular_velocity += rotationSign * ANGULAR_ACCELERATION;
            angular_velocity = glm::clamp(angular_velocity, -MAX_ANGULAR_SPEED, MAX_ANGULAR_SPEED);

            RotateAroundPoint(pivotPoint, axis, angular_velocity);
        } else {
            angular_velocity = 0.0f;
        }
        vertical_velocity = 0.0f;
    }

    glm::vec3 bikePos = transform.GetPosition();
    bikePos.y += vertical_velocity;
    transform.SetPosition(bikePos);
}
