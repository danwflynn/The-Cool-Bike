#pragma once
#include "CameraController.h"
#include "math/Transform.h"
#include "entities/Bike.h"

class FollowCameraController : public CameraController {
public:
    FollowCameraController(const Bike& targetBike);

    void Update(Camera& camera, float dt) override;

private:
    const Bike& targetBike;

    float followDistance = 9.5f;
    float height = 1.2f;
    float smoothing = 6.0f;
};
