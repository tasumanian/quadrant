#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
    velocity =
        glm::vec3(0.0f);

    mass = 1.0f;

    useGravity = true;

	isGrounded = false;
}
void Rigidbody::AdaptGravity(float dt)
{
    velocity.y -= gravity * dt;
}