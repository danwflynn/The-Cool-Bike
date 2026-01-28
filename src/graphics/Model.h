#pragma once

#include <string>
#include <vector>
#include "Mesh.h"

class Shader;

class Model {
public:
    Model(const std::string& path);

    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;

    void LoadModel(const std::string& path);
};
