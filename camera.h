#pragma once

#include "mat4.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class Camera
{
    public:
        
        Transform transform;

        float yaw;
        float pitch;

        Camera();

        glm::mat4 GetViewMatrix() const;
};