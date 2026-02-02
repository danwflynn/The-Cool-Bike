#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "entities/Bike.h"


class WheelHitboxTests : public ::testing::Test {
protected:
    Transform transform;
    std::optional<WheelHitbox> wheel;
    
    WheelHitboxTests() {}

    void SetUp() override {
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
                posVertices.reserve(mesh->mNumVertices);
                for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                    if (mesh->mVertices[v].z >= 0) posVertices.push_back(glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z));
                }
                wheel.emplace(posVertices, transform);
            }
        }
        transform.SetScale(glm::vec3(Bike::SCALE));
    }
};


TEST_F(WheelHitboxTests, GetWorldCenterWithTransformAtOrigin) {
    ASSERT_TRUE(wheel.has_value());
    std::cout << glm::to_string(wheel->GetWorldCenter()) << std::endl;
}
