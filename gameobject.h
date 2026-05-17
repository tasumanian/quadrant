#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Rigidbody.h"

class GameObject
{
public:
	Rigidbody* rigidbody;

    Transform transform;

    Material* material;

    Mesh* mesh;

public:

    GameObject();
};