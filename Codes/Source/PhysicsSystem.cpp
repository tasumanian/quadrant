#include "PhysicsSystem.h"
#include "Ray.h"
#include "GameObject.h"
#include <iostream>

void PhysicsSystem::FixedUpdate(Scene& scene, float dt)
{
	Gravity(scene, dt);
    Integrate(scene, dt);
	DetectCollision(scene);
    ResolveAABB(scene);
}
void PhysicsSystem::DetectCollision(Scene& scene)
{
	auto& objects = scene.GetObjects();

	for (size_t i = 0; i < objects.size(); ++i)
	{
		GameObject& obj = *objects[i].get();

		if (!obj.HasComponent<Rigidbody>())
			continue;

		Rigidbody& rb = *obj.GetComponent<Rigidbody>();

		Transform& tr = *obj.GetTransform();

		// 衝突判定
		for (size_t j = i + 1; j < objects.size(); ++j)
		{
			GameObject& other = *objects[j].get();

			if (!other.HasComponent<BoxCollider>())
				continue;

            CollisionInfo info;

			// 衝突
			if (CheckAABB(obj, other, info))
            {
                m_collisions.push_back(CollisionPair{&obj,&other,info});
            }

			//ResolveAABB(obj, other, rb);

		}
	}
}
AABB PhysicsSystem::GetWorldAABB(
    GameObject& obj)
{
    Transform* transform = obj.GetTransform();
    BoxCollider* collider = obj.GetComponent<BoxCollider>();

    glm::mat4 worldMatrix = transform->GetWorldMatrix();

    // Colliderのローカル中心をワールド座標に変換
    glm::vec3 center = glm::vec3(
        worldMatrix * glm::vec4(collider->center, 1.0f)
    );

    glm::vec3 localHalf = collider->size * 0.5f;

    // ワールド行列の各軸の拡大率と回転を反映
    glm::vec3 axisX = glm::vec3(worldMatrix[0]);
    glm::vec3 axisY = glm::vec3(worldMatrix[1]);
    glm::vec3 axisZ = glm::vec3(worldMatrix[2]);

    glm::vec3 worldHalf;

    worldHalf.x =
        std::abs(axisX.x) * localHalf.x +
        std::abs(axisY.x) * localHalf.y +
        std::abs(axisZ.x) * localHalf.z;

    worldHalf.y =
        std::abs(axisX.y) * localHalf.x +
        std::abs(axisY.y) * localHalf.y +
        std::abs(axisZ.y) * localHalf.z;

    worldHalf.z =
        std::abs(axisX.z) * localHalf.x +
        std::abs(axisY.z) * localHalf.y +
        std::abs(axisZ.z) * localHalf.z;

    return AABB{
        center - worldHalf,
        center + worldHalf,
        center,
        worldHalf
    };
}
void PhysicsSystem::Gravity(Scene& scene, float dt)
{
    auto& objects =
        scene.GetObjects();

    for (auto& object : objects)
    {
		GameObject* obj = object.get();
        if (!obj->HasComponent<Rigidbody>())
            continue;

        Rigidbody* rb = obj->GetComponent<Rigidbody>();

        rb->isGrounded = false;

        Transform& tr = *obj->GetTransform();
        //テスト用
        //tr.rotation =
        //    glm::normalize(glm::angleAxis(glm::radians(90.0f * dt), glm::vec3(0, 1, 0))
//         \\       * tr.rotation);

        if (rb->isKinematic)
            continue;

        // 重力
        if (rb->useGravity)
        {
            rb->AdaptGravity(dt);
        }
    }
}
void PhysicsSystem::Integrate(Scene& scene, float dt)
{
	auto& objects =
		scene.GetObjects();
	for (auto& obj : objects)
	{
		if (!obj->HasComponent<Rigidbody>())
			continue;

        if (obj->GetComponent<Rigidbody>()->isKinematic)
            continue;

        obj->GetTransform()->position += 
            obj->GetComponent<Rigidbody>()->velocity * dt;
	}
}
bool PhysicsSystem::CheckAABB(GameObject& obj1 , GameObject& obj2, CollisionInfo& info)
{

    if (!obj1.HasComponent<BoxCollider>() ||
        !obj2.HasComponent<BoxCollider>())
        return false;

    AABB a = GetWorldAABB(obj1);
    AABB b = GetWorldAABB(obj2);

    glm::vec3 overlap =
        glm::min(a.max, b.max) -
        glm::max(a.min, b.min);

    if (overlap.x <= 0.0f ||
        overlap.y <= 0.0f ||
        overlap.z <= 0.0f)
    {
        return false;
    }

    info.isColliding = true;
    info.other = &obj2;

    // 最も重なりが小さい軸を衝突法線にする
    if (overlap.x <= overlap.y &&
        overlap.x <= overlap.z)
    {
        info.penetration = overlap.x;

        info.normal = glm::vec3(
            b.center.x >= a.center.x ? 1.0f : -1.0f,
            0.0f,
            0.0f
        );
    }
    else if (overlap.y <= overlap.x &&
        overlap.y <= overlap.z)
    {
        info.penetration = overlap.y;

        info.normal = glm::vec3(
            0.0f,
            b.center.y >= a.center.y ? 1.0f : -1.0f,
            0.0f
        );
    }
    else
    {
        info.penetration = overlap.z;

        info.normal = glm::vec3(
            0.0f,
            0.0f,
            b.center.z >= a.center.z ? 1.0f : -1.0f
        );
    }

    return true;
}
void PhysicsSystem::ResolveAABB(Scene& scene)
{
	auto& objects = scene.GetObjects();

    for (auto& object : objects)
    {
        if (object->HasComponent<Rigidbody>())
        {
            Rigidbody* rb = object->GetComponent<Rigidbody>();

            if (rb != nullptr)
                rb->isGrounded = false;
        }
    }
    for (const CollisionPair& pair : m_collisions)
    {
        if (pair.a == nullptr || pair.b == nullptr)
            continue;

        GameObject& a = *pair.a;
        GameObject& b = *pair.b;

        const CollisionInfo& info = pair.info;

        if (!info.isColliding || info.penetration <= 0.0f)
            continue;

        Transform* transformA = a.GetTransform();
        Transform* transformB = b.GetTransform();

        if (transformA == nullptr || transformB == nullptr)
            continue;

        Rigidbody* rbA = a.GetComponent<Rigidbody>();
        Rigidbody* rbB = b.GetComponent<Rigidbody>();

        // Rigidbodyを持つオブジェクトだけ移動させる
        const bool movableA = rbA != nullptr && !rbA->isKinematic;
        const bool movableB = rbB != nullptr && !rbB->isKinematic;

        if (!movableA && !movableB)
            continue;

        const glm::vec3 normal = info.normal;
        const float penetration = info.penetration;

        glm::vec3 worldPosA = transformA->GetWorldPosition();
        glm::vec3 worldPosB = transformB->GetWorldPosition();

        if (movableA && movableB)
        {
            transformA->SetWorldPosition(
                worldPosA - normal * (penetration * 0.5f)
            );

            transformB->SetWorldPosition(
                worldPosB + normal * (penetration * 0.5f)
            );
        }
        else if (movableA)
        {
            transformA->SetWorldPosition(
                worldPosA - normal * penetration
            );
        }
        else
        {
            transformB->SetWorldPosition(
                worldPosB + normal * penetration
            );
        }

        // 法線の向きから接地状態を判定する
        // normalはAからBへ向かう方向
        if (movableA && normal.y < -0.5f)
        {
            rbA->isGrounded = true;
        }

        if (movableB && normal.y > 0.5f)
        {
            rbB->isGrounded = true;
        }
        ResolveCollisionVelocity(a, b, info.normal);
    }
}
void PhysicsSystem::ResolveCollisionVelocity(
    GameObject& a,
    GameObject& b,
    const glm::vec3& normal)
{
    Rigidbody* rbA = a.GetComponent<Rigidbody>();
    Rigidbody* rbB = b.GetComponent<Rigidbody>();

    const bool movableA =
        rbA != nullptr && !rbA->isKinematic;

    const bool movableB =
        rbB != nullptr && !rbB->isKinematic;

    if (!movableA && !movableB)
        return;

    glm::vec3 velocityA =
        movableA ? rbA->velocity : glm::vec3(0.0f);

    glm::vec3 velocityB =
        movableB ? rbB->velocity : glm::vec3(0.0f);

    // AからBへ向かう法線方向の相対速度
    float relativeNormalVelocity =
        glm::dot(velocityB - velocityA, normal);

    // すでに離れている場合は何もしない
    if (relativeNormalVelocity >= 0.0f)
        return;

    // 逆質量
    float invMassA =
        movableA && rbA->mass > 0.0f
        ? 1.0f / rbA->mass : 0.0f;

    float invMassB =
        movableB && rbB->mass > 0.0f
        ? 1.0f / rbB->mass : 0.0f;

    float totalInvMass = invMassA + invMassB;

    if (totalInvMass <= 0.0f)
        return;

    // 反発係数は小さい方を採用
    float restitution = 0.0f;

    if (movableA && movableB)
    {
        restitution = glm::min(
            rbA->restitution,
            rbB->restitution
        );
    }
    else if (movableA)
    {
        restitution = rbA->restitution;
    }
    else
    {
        restitution = rbB->restitution;
    }

    restitution = glm::clamp(restitution, 0.0f, 1.0f);

    // 衝撃量
    float impulseMagnitude =
        -(1.0f + restitution) * relativeNormalVelocity
        / totalInvMass;

    glm::vec3 impulse =
        normal * impulseMagnitude;

    if (movableA)
        rbA->velocity -= impulse * invMassA;

    if (movableB)
        rbB->velocity += impulse * invMassB;
}
bool PhysicsSystem::CheckRayAABB(const Ray& ray,GameObject& obj,float& distance
)
{
    if (!obj.HasComponent<BoxCollider>())
        return false;


    glm::vec3 min =
        obj.GetTransform()->position
        - obj.GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 max =
        obj.GetTransform()->position
        + obj.GetComponent<BoxCollider>()->size * 0.5f;

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    for (int i = 0; i < 3; i++)
    {
        float origin =
            ray.origin[i];

        float dir =
            ray.direction[i];

        float boxMin =
            min[i];

        float boxMax =
            max[i];

        if (fabs(dir) < 0.00001f)
        {
            if (origin < boxMin ||
                origin > boxMax)
            {
                return false;
            }

            continue;
        }

        float t1 =
            (boxMin - origin) / dir;

        float t2 =
            (boxMax - origin) / dir;

        if (t1 > t2)
            std::swap(t1, t2);

        tMin =
            std::max(tMin, t1);

        tMax =
            std::min(tMax, t2);

        if (tMin > tMax)
            return false;
    }

    distance = tMin;

    return true;
}
GameObject* PhysicsSystem::Raycast(
    Scene& scene,
    const Ray& ray)
{
    GameObject* result = nullptr;

    float nearest = FLT_MAX;

    for (auto& object : scene.GetObjects())
    {
        GameObject* obj = object.get();

        float distance;

        if (CheckRayAABB(
            ray,
            *obj,
            distance))
        {
            if (distance < nearest)
            {
                nearest = distance;
                result = obj;
            }
        }
    }

    return result;
}