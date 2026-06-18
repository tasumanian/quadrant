#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include <string>

class GameObject
{
public:
	Rigidbody rigidbody;

    Transform transform;

    Material material;

    Mesh* mesh;

	BoxCollider boxCollider;

    std::string name;

public:

    GameObject();
};