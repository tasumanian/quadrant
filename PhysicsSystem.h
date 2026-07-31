#pragma once
#include "Renderer.h"
#include "Scene.h"

class PhysicsSystem
{
public:
	void Gravity(Scene* scene, float dt);
	bool CheckAABB(const GameObject& obj1, const GameObject& obj2);
	void ResolveAABB(GameObject& obj1, GameObject& obj2, Rigidbody& rb);

};