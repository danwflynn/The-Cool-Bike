#include "Bike.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Bike::Bike(Camera& cameraRef) : camera(cameraRef), vertical_velocity(0.0f) {
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
    transform.SetRotation(glm::vec3(glm::radians(0.0f), 0.0f, 0.0f));
}

Transform& Bike::getTransform() {
    return transform;
}

bool Bike::IsCollidingWithGround() const {
    bool back = backWheel && backWheel->IsCollidingWithGround();
    bool front = frontWheel && frontWheel->IsCollidingWithGround();
    return back || front;
}

void Bike::applyVerticalForce() {
    if (!IsCollidingWithGround()) {
        vertical_velocity -= GRAVITATIONAL_ACCELERATION;
        vertical_velocity = std::max(vertical_velocity, MAX_FALL_SPEED);
    } else {
        vertical_velocity = 0.0f;
    }

    glm::vec3 bikePos = transform.GetPosition();
    bikePos.y += vertical_velocity;
    transform.SetPosition(bikePos);
}
