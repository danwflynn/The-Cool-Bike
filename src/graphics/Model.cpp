#include "Model.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

Model::Model(const std::string& path) {
    LoadModel(path);
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality
    );

    if (!scene || !scene->mRootNode || scene->mNumMeshes == 0) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    meshes.reserve(scene->mNumMeshes);

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        vertices.reserve(mesh->mNumVertices * 6);

        // ---- Vertices ----
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            // Position
            vertices.push_back(mesh->mVertices[v].x);
            vertices.push_back(mesh->mVertices[v].y);
            vertices.push_back(mesh->mVertices[v].z);

            // Normal (guaranteed by aiProcess_GenNormals)
            vertices.push_back(mesh->mNormals[v].x);
            vertices.push_back(mesh->mNormals[v].y);
            vertices.push_back(mesh->mNormals[v].z);
        }

        // ---- Indices ----
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            const aiFace& face = mesh->mFaces[f];

            // Triangulated, so always 3
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        meshes.emplace_back(
            vertices.data(),
            vertices.size() * sizeof(float),
            indices.data(),
            indices.size() * sizeof(unsigned int),
            6 * sizeof(float)
        );
    }

    std::cout << "Loaded meshes: " << meshes.size() << std::endl;
}

void Model::Draw() {
    for (const auto& mesh : meshes) {
        mesh.Draw();
    }
}
