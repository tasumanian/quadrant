#pragma once

#include <glm/glm.hpp>
#include "Component/Component.h"

class BoxCollider : public Component
{
public:
	BoxCollider();

	glm::vec3 size;

};
