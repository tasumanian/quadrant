#pragma once
#include "Component/Component.h"
#include <glm/glm.hpp>

class Ray;

class CameraComponent : public Component
{
public:

	float fov = 60.0f;

	float nearClip = 0.1f;

	float farClip = 100.0f;

	bool isMainCamera = true;

	bool orthographic = false;

	float orthographicSize = 5.0f;

	bool enabled = true;

	int priority = 0;

	glm::vec4 viewport =
	{
		0,
		0,
		1,
		1
	};
};