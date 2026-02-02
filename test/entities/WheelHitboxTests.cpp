#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/gtc/constants.hpp>

#include "entities/Bike.h"


constexpr float EPS = 1e-5f;

class WheelHitboxTests : public ::testing::Test {
protected:
    Transform transform;
    std::optional<WheelHitbox> frontWheel;
    std::optional<WheelHitbox> backWheel;
    
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
        transform.SetScale(glm::vec3(Bike::SCALE));
    }
};


TEST_F(WheelHitboxTests, GetWorldCenterAlignedWithTransform) {
    ASSERT_TRUE(frontWheel.has_value());
    ASSERT_TRUE(backWheel.has_value());
    glm::vec3 originalFrontWorldCenter = frontWheel->GetWorldCenter();
    glm::vec3 originalBackWorldCenter = backWheel->GetWorldCenter();
    std::cout << "Starting front wheel center: " << glm::to_string(frontWheel->GetWorldCenter()) << std::endl;
    std::cout << "Starting back wheel center: " << glm::to_string(backWheel->GetWorldCenter()) << std::endl;
    transform.SetPosition(glm::vec3(1.0f, 1.0f, 1.0f));
    EXPECT_FLOAT_EQ(frontWheel->GetWorldCenter().x, originalFrontWorldCenter.x + 1.0f);
    EXPECT_FLOAT_EQ(frontWheel->GetWorldCenter().y, originalFrontWorldCenter.y + 1.0f);
    EXPECT_FLOAT_EQ(frontWheel->GetWorldCenter().z, originalFrontWorldCenter.z + 1.0f);
    EXPECT_FLOAT_EQ(backWheel->GetWorldCenter().x, originalBackWorldCenter.x + 1.0f);
    EXPECT_FLOAT_EQ(backWheel->GetWorldCenter().y, originalBackWorldCenter.y + 1.0f);
    EXPECT_FLOAT_EQ(backWheel->GetWorldCenter().z, originalBackWorldCenter.z + 1.0f);
    std::cout << "Ending front wheel center: " << glm::to_string(frontWheel->GetWorldCenter()) << std::endl;
    std::cout << "Ending back wheel center: " << glm::to_string(backWheel->GetWorldCenter()) << std::endl;
}

TEST_F(WheelHitboxTests, GetWorldCenterAlignedWithYAxisRotation) {
    ASSERT_TRUE(frontWheel.has_value());
    ASSERT_TRUE(backWheel.has_value());
    glm::vec3 originalFrontWorldCenter = frontWheel->GetWorldCenter();
    glm::vec3 originalBackWorldCenter = backWheel->GetWorldCenter();
    std::cout << "Starting front wheel center: " << glm::to_string(frontWheel->GetWorldCenter()) << std::endl;
    std::cout << "Starting back wheel center: " << glm::to_string(backWheel->GetWorldCenter()) << std::endl;
    transform.SetRotation(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
    EXPECT_NEAR(frontWheel->GetWorldCenter().x, originalFrontWorldCenter.z, EPS);
    EXPECT_NEAR(frontWheel->GetWorldCenter().y, originalFrontWorldCenter.y, EPS);
    EXPECT_NEAR(frontWheel->GetWorldCenter().z, originalFrontWorldCenter.x, EPS);
    EXPECT_NEAR(backWheel->GetWorldCenter().x, originalBackWorldCenter.z, EPS);
    EXPECT_NEAR(backWheel->GetWorldCenter().y, originalBackWorldCenter.y, EPS);
    EXPECT_NEAR(backWheel->GetWorldCenter().z, originalBackWorldCenter.x, EPS);
    std::cout << "Ending front wheel center: " << glm::to_string(frontWheel->GetWorldCenter()) << std::endl;
    std::cout << "Ending back wheel center: " << glm::to_string(backWheel->GetWorldCenter()) << std::endl;
}
