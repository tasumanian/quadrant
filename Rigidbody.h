#pragma once

#include <glm/glm.hpp>

class Rigidbody
{
public:

    glm::vec3 velocity;

    float mass;

    const float gravity = 9.8f;

    bool useGravity;

public:
    void AdaptGravity(float dt);
    Rigidbody();
};