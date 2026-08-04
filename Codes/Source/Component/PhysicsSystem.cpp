#include "PhysicsSystem.h"
#include "Ray.h"
#include <iostream>
void PhysicsSystem::Gravity(Scene* scene, float dt)
{
    auto& objects =
		scene->GetObjects();

    for (GameObject& obj:objects)
    {
        if(!obj.HasComponent<Rigidbody>())
            continue;

        Rigidbody& rb = *obj.GetComponent<Rigidbody>();
		
		rb.isGrounded = false;


        //テスト用
        obj.transform.rotation = 
            glm::normalize(glm::angleAxis(glm::radians(90.0f * dt), glm::vec3(0, 1, 0))
            * obj.transform.rotation);


        // 重力
        if (rb.useGravity)
        {
            rb.AdaptGravity(dt);
        }
        obj.transform.position += rb.velocity * dt;

        // 衝突判定
        for ( GameObject& other : objects)
        {

            // 自分自身除外
            if (&obj == &other)
            {
                continue;
            }

            if(!other.HasComponent<BoxCollider>() || !obj.HasComponent<BoxCollider>())
				continue;

            // 衝突
            if (!CheckAABB(obj, other))
                continue;

            ResolveAABB(obj, other, rb);
         }   
    }
}
bool PhysicsSystem::CheckAABB(GameObject& obj1, GameObject& obj2)
{
    if (!obj1.HasComponent<BoxCollider>() || !obj2.HasComponent<BoxCollider>())
        return false;


    glm::vec3 aMin = obj1.transform.position - obj1.GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 aMax = obj1.transform.position + obj1.GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 bMin = obj2.transform.position - obj2.GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 bMax = obj2.transform.position + obj2.GetComponent<BoxCollider>()->size * 0.5f;

    return //衝突確認
    (//AとBの座標の最大点と最小点を比較して、重なっているかどうかを確認
    aMin.x <= bMax.x && aMax.x >= bMin.x &&

    aMin.y <= bMax.y && aMax.y >= bMin.y &&

    aMin.z <= bMax.z && aMax.z >= bMin.z
    );
}
void PhysicsSystem::ResolveAABB(GameObject& obj1, GameObject& obj2 , Rigidbody& rb)
{
    glm::vec3 aMin = obj1.transform.position - obj1.GetComponent<BoxCollider>()->size * 0.5f;
    glm::vec3 aMax = obj1.transform.position + obj1.GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 bMin = obj2.transform.position - obj2.GetComponent<BoxCollider>()->size * 0.5f;
    glm::vec3 bMax = obj2.transform.position + obj2.GetComponent<BoxCollider>()->size * 0.5f;

    float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);
    float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
    float overlapZ = std::min(aMax.z, bMax.z) - std::max(aMin.z, bMin.z);

    float minOverlap = std::min(overlapY, overlapX);
    minOverlap = std::min(minOverlap, overlapZ);

    // obj が上にいる時だけ
    if (minOverlap == overlapY)
    {
        if (obj1.transform.position.y < obj2.transform.position.y)
        {
            obj1.transform.position.y -= overlapY;
        }
        else
        {
            obj1.transform.position.y += overlapY;

            rb.isGrounded = true;
        }

        rb.velocity.y = 0.0f;
    }
    else if (minOverlap == overlapX)
    {
        if (obj1.transform.position.x < obj2.transform.position.x)
        {
            obj1.transform.position.x -= overlapX;
        }
        else
        {
            obj1.transform.position.x += overlapX;
        }

        rb.velocity.x = 0.0f;
    }
    else
    {
        if (obj1.transform.position.z < obj2.transform.position.z)
        {
            obj1.transform.position.z -= overlapZ;
        }
        else
        {
            obj1.transform.position.z += overlapZ;
        }

        rb.velocity.z = 0.0f;
    }
}
bool PhysicsSystem::CheckRayAABB(
    const Ray& ray,
    GameObject& obj,
    float& distance
)
{
    if (!obj.HasComponent<BoxCollider>())
        return false;


    glm::vec3 min =
        obj.transform.position
        - obj.GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 max =
        obj.transform.position
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
    Scene* scene,
    const Ray& ray)
{
    GameObject* result = nullptr;

    float nearest = FLT_MAX;

    for (GameObject& obj : scene->GetObjects())
    {
        float distance;

        if (CheckRayAABB(
            ray,
            obj,
            distance))
        {
            if (distance < nearest)
            {
                nearest = distance;
                result = &obj;
            }
        }
    }

    return result;
}