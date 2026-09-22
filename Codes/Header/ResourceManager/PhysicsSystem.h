#pragma once
#include "Renderer.h"
#include "Scene.h"

class Rigidbody;

class PhysicsSystem
{
public:
	void Gravity(Scene* scene, float dt);
	bool CheckAABB(std::unique_ptr<GameObject>& obj1, std::unique_ptr<GameObject>& obj2);
	void ResolveAABB(std::unique_ptr<GameObject>& obj1, std::unique_ptr<GameObject>& obj2, Rigidbody& rb);
	std::unique_ptr<GameObject>* Raycast(Scene* scene, const Ray& ray);
	bool CheckRayAABB(const Ray& ray, std::unique_ptr<GameObject>& obj, float& distance);
};