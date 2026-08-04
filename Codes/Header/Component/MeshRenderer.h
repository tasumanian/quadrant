#pragma once
#include "Component/Component.h"
#include "ResourceManager/Mesh.h"
#include "ResourceManager/Material.h"

class MeshRenderer : public Component
{
public:

    Mesh* mesh;

    Material* material;



	MeshRenderer();
};