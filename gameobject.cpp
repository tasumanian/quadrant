#include "GameObject.h"

GameObject::GameObject()
{
    rigidbody = nullptr;
    mesh = nullptr;
	material = nullptr;
	boxCollider = nullptr;
	name = "GameObject";
}