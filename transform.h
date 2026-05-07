#pragma once

#include "mat4.h"

struct Transform
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float rotationY = 0.0f;

	Mat4 ToMatrix() const;
};