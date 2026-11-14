#pragma once

#include <glm/glm.hpp>
#include "Component.h"

class BoxCollider : public Component
{
public:
	BoxCollider();

	glm::vec3 size;
	glm::vec3 center;

	const char* GetComponentName() const override
	{
		return "BoxCollider";
	}

};
