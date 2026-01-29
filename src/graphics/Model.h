#pragma once

#include <string>
#include <vector>
#include "Mesh.h"


class Model {
public:
    Model(const std::string& path);

    void Draw();
    float getBottomY();

private:
    std::vector<Mesh> meshes;
    float minY;
    void LoadModel(const std::string& path);
};
