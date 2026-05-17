#pragma once

#include <glm/glm.hpp>

class Rigidbody
{
public:

    glm::vec3 velocity;

    float mass;

    bool useGravity;

public:

    Rigidbody();
};