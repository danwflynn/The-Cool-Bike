#include <string>

enum class SplitAxis {
    None,
    X,
    Y,
    Z
};

struct MeshSplitRule {
    std::string meshName;   // empty = apply to all meshes
    SplitAxis axis = SplitAxis::None;
    float value = 0.0f;
};
