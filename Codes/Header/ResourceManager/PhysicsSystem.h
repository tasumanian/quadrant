#pragma once
#include "Renderer.h"
#include "Scene.h"

class Rigidbody;

class PhysicsSystem
{
public:
	void Gravity(Scene* scene, float dt);
	bool CheckAABB(GameObject& obj1, GameObject& obj2);
	void ResolveAABB(GameObject& obj1, GameObject& obj2, Rigidbody& rb);
	GameObject* Raycast(Scene* scene, const Ray& ray);
	bool CheckRayAABB(const Ray& ray, GameObject& obj, float& distance);
};