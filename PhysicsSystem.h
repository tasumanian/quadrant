#pragma once
#include "Renderer.h"
#include "Scene.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	void Gravity(Scene* scene, float dt);
	bool CheckAABB(const GameObject& obj1, const GameObject& obj2);


};