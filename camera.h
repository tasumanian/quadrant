#pragma once

#include "mat4.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class Camera
{
    public:
	    glm::vec3 position; //カメラの位置

	    glm::vec3 forward; //カメラの視線方向
        glm::vec3 up; //カメラの上方向
        glm::vec3 right; //カメラの右方向

        float yaw;
        float pitch;


        Camera();

        void UpdateVectors();

        glm::mat4 GetViewMatrix() const;
};