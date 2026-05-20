#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
    velocity =
        glm::vec3(1.0f);

    mass = 1.0f;

    useGravity = true;
}
void Rigidbody::AdaptGravity(float dt)
{
    velocity.y -= gravity * dt;
}