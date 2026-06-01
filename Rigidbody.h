#pragma once

#include <glm/glm.hpp>

class Rigidbody
{
public:

    glm::vec3 velocity;

    float mass;

    static constexpr float gravity = 9.8f;

    bool useGravity;

    bool isGrounded;

public:
    void AdaptGravity(float dt);
    Rigidbody();
};