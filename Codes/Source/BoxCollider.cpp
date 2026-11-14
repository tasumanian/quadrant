#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	size = glm::vec3(1.0f, 1.0f, 1.0f);
    center = glm::vec3(0.0f);

    RegisterProperty(
        "Enable",
        &enabled
    );

    RegisterProperty(
        "Size",
        &size
    );
}