#pragma once

#include "Transform.h"
#include "Mesh.h"

class GameObject
{
public:

    Transform transform;

    Mesh* mesh;

public:

    GameObject();
};