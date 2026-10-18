#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
    velocity =
        glm::vec3(0.0f);

    mass = 1.0f;

    useGravity = true;

	isGrounded = false;

	isKinematic = false;

    RegisterProperty(
        "Enable",
        &enabled
    );

    RegisterProperty(
        "Use Gravity",
        &useGravity
    );

    RegisterProperty(
        "Mass",
        &mass
    );

    RegisterProperty(
        "Velocity",
        &velocity
    );
}
void Rigidbody::AdaptGravity(float dt)
{
    velocity.y -= gravity * dt;
}
void Rigidbody::AddForce(const glm::vec3& force)
{
    if (isKinematic)
        return;

	velocity += force / mass;
}