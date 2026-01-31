#pragma once

#include <string>
#include <vector>
#include "Mesh.h"
#include "util/SplitRules.h"

class Model {
public:
    // Original behavior still works
    Model(const std::string& path);

    // New behavior: optional mesh splitting
    Model(
        const std::string& path,
        const std::vector<MeshSplitRule>& splitRules
    );

    void Draw();
    std::vector<Mesh>& getMeshes();

private:
    std::vector<Mesh> meshes;

    void LoadModel(const std::string& path);
    void LoadModel(const std::string& path, const std::vector<MeshSplitRule>& splitRules);
};
