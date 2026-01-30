#include "Bike.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Bike::Bike(Camera& cameraRef) : camera(cameraRef) {
    minY = std::numeric_limits<float>::max();
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

        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            if (minY > mesh->mVertices[v].y) minY = mesh->mVertices[v].y;
        }
    }

    transform.SetScale(glm::vec3(SCALE));
    transform.SetPosition(glm::vec3(0.0f, -1 * minY * SCALE, -2.0f));
    transform.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
}

Transform& Bike::getTransform() {
    return transform;
}
