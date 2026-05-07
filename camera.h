#pragma once

#include "mat4.h"

class Camera
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Mat4 GetViewMatrix() const;
};