#pragma once
#include "GameObject.h"
#include "Camera.h"
class CharacterController
{
public:

    float moveSpeed;

    CharacterController();

    void CameraUpdate(
        Camera& camera,
        float deltaTime
    );

};
