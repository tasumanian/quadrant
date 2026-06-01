#pragma once
#include "GameObject.h"
class CharacterController
{
public:

    float moveSpeed;

    CharacterController();

    void Update(
        GameObject& player,
        float yaw,
        float deltaTime
    );

};
