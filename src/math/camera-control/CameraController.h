#pragma once
#include "math/Camera.h"

class CameraController {
public:
    virtual ~CameraController() = default;
    virtual void Update(Camera& camera, float dt) = 0;
};
