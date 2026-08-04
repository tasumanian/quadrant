#pragma once
#include "Component/CameraComponent.h"
class CharacterController
{
public:

    float moveSpeed;

    float m_yaw = 90.0f;
    float m_pitch = 0.0f;

    CharacterController();

    void Update(
        CameraComponent* camera,
        float deltaTime
    );

};
