#pragma once

#include <glm/glm.hpp>

class Ray
{
public:

    glm::vec3 origin;

    glm::vec3 direction;

public:

    Ray() = default;

    Ray(
        const glm::vec3& o,
        const glm::vec3& d)
        :
        origin(o),
        direction(glm::normalize(d))
    {
    }
};