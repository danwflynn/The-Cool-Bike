#include "Model.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


Model::Model(const std::string& path) {
    LoadModel(path);
}

Model::Model(
    const std::string& path,
    const std::vector<MeshSplitRule>& splitRules
) {
    LoadModel(path, splitRules);
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

        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            vertices.push_back(mesh->mVertices[v].x);
            vertices.push_back(mesh->mVertices[v].y);
            vertices.push_back(mesh->mVertices[v].z);

            vertices.push_back(mesh->mNormals[v].x);
            vertices.push_back(mesh->mNormals[v].y);
            vertices.push_back(mesh->mNormals[v].z);
        }

        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            const aiFace& face = mesh->mFaces[f];
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


void Model::LoadModel(const std::string& path, const std::vector<MeshSplitRule>& rules) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality
    );

    meshes.reserve(scene->mNumMeshes * 2);

    if (!scene || !scene->mRootNode || scene->mNumMeshes == 0) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        std::string meshName = mesh->mName.C_Str();

        // Find rule
        const MeshSplitRule* rule = nullptr;
        for (const auto& r : rules) {
            if (r.axis != SplitAxis::None && r.meshName == meshName) {
                rule = &r;
                break;
            }
        }

        if (!rule) {
            std::cout << "No rule for " << meshName << std::endl;
            std::vector<float> vertices;
            std::vector<unsigned int> indices;

            vertices.reserve(mesh->mNumVertices * 6);

            for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
                vertices.push_back(mesh->mVertices[v].x);
                vertices.push_back(mesh->mVertices[v].y);
                vertices.push_back(mesh->mVertices[v].z);

                vertices.push_back(mesh->mNormals[v].x);
                vertices.push_back(mesh->mNormals[v].y);
                vertices.push_back(mesh->mNormals[v].z);
            }

            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                const aiFace& face = mesh->mFaces[f];
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

            continue;
        }

        std::cout << "Got to rule for " << meshName << std::endl;
        std::vector<float> vertsPos, vertsNeg;
        std::vector<unsigned int> indsPos, indsNeg;

        auto axisValue = [&](const aiVector3D& v) {
            switch (rule->axis) {
            case SplitAxis::X: return v.x;
            case SplitAxis::Y: return v.y;
            case SplitAxis::Z: return v.z;
            default: return 0.0f;
            }
        };

        unsigned int posIndex = 0;
        unsigned int negIndex = 0;

        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            const aiFace& face = mesh->mFaces[f];

            float avg = 0.0f;
            for (int j = 0; j < 3; j++)
                avg += axisValue(mesh->mVertices[face.mIndices[j]]);
            avg /= 3.0f;

            bool positive = avg > rule->value;

            auto& verts = positive ? vertsPos : vertsNeg;
            auto& inds  = positive ? indsPos  : indsNeg;
            auto& base  = positive ? posIndex : negIndex;

            for (int j = 0; j < 3; j++) {
                unsigned int idx = face.mIndices[j];
                const auto& p = mesh->mVertices[idx];
                const auto& n = mesh->mNormals[idx];

                verts.push_back(p.x);
                verts.push_back(p.y);
                verts.push_back(p.z);
                verts.push_back(n.x);
                verts.push_back(n.y);
                verts.push_back(n.z);

                inds.push_back(base++);
            }
        }

        if (!vertsPos.empty()) {
            meshes.emplace_back(
                vertsPos.data(),
                vertsPos.size() * sizeof(float),
                indsPos.data(),
                indsPos.size() * sizeof(unsigned int),
                6 * sizeof(float)
            );
        }

        if (!vertsNeg.empty()) {
            meshes.emplace_back(
                vertsNeg.data(),
                vertsNeg.size() * sizeof(float),
                indsNeg.data(),
                indsNeg.size() * sizeof(unsigned int),
                6 * sizeof(float)
            );
        }
    }
}

void Model::Draw() {
    for (const auto& mesh : meshes) {
        mesh.Draw();
    }
}
