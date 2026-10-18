#pragma once
#include "Renderer.h"
#include "Scene.h"

class Rigidbody;

struct CollisionInfo
{
	bool isColliding = false;

	// AからBへ向かう衝突法線
	glm::vec3 normal = glm::vec3(0.0f);

	// AとBが重なっている深さ
	float penetration = 0.0f;

	// 衝突相手
	GameObject* other = nullptr;
};

struct CollisionPair
{
	GameObject* a = nullptr;
	GameObject* b = nullptr;
	CollisionInfo info;
};

class PhysicsSystem
{
public:

	void FixedUpdate(Scene& scene, float dt);
	void Gravity(Scene& scene, float dt);
	void DetectCollision(Scene& scene);
	void Integrate(Scene& scene, float dt);
	bool CheckAABB(GameObject& obj1, GameObject& obj2 , CollisionInfo info);
	void ResolveAABB(Scene& scene);
	void ResolveCollisionVelocity(GameObject& a,GameObject& b,const glm::vec3& normal);
	GameObject* Raycast(Scene& scene, const Ray& ray);
	bool CheckRayAABB(const Ray& ray, GameObject& obj, float& distance);

private:
	std::vector<CollisionPair> m_collisions;

};