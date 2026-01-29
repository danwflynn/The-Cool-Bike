#pragma once
#include "math/Transform.h"
#include "math/Camera.h"


class Bike {
public:
    static constexpr float SCALE = 0.01f;

    Bike(Camera& cameraRef, float bottomY);

    Transform& getTransform();
private:
    Transform transform;
    Camera& camera;
};
