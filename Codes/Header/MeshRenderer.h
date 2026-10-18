#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer : public Component
{
public:

    Mesh* mesh;

    Material* material;

    const char* GetComponentName() const override
    {
        return "MeshRenderer";
    }

	MeshRenderer();
};