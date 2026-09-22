#include "ResourceManager/PhysicsSystem.h"
#include "ResourceManager/Ray.h"
#include "ResourceManager/GameObject.h"
#include <iostream>

void PhysicsSystem::Gravity(Scene* scene, float dt)
{
    auto& objects =
        scene->GetObjects();

    for (auto& obj : objects)
    {

        if(!obj->HasComponent<Rigidbody>())
            continue;

        Rigidbody& rb = *obj->GetComponent<Rigidbody>();

        rb.isGrounded = false;

        Transform& tr = *obj->GetComponent<Transform>();
        //テスト用
        tr.rotation =
            glm::normalize(glm::angleAxis(glm::radians(90.0f * dt), glm::vec3(0, 1, 0))
            * tr.rotation);


        // 重力
        if (rb.useGravity)
        {
            rb.AdaptGravity(dt);
        }
        tr.position += rb.velocity * dt;

        // 衝突判定
        for (auto& other : objects)
        {

            // 自分自身除外
            if (obj == other)
            {
                continue;
            }

            if(!other->HasComponent<BoxCollider>() || !obj->HasComponent<BoxCollider>())
                continue;

            // 衝突
            if (!CheckAABB(obj, other))
                continue;

            ResolveAABB(obj, other, rb);
         }   
    }
}
bool PhysicsSystem::CheckAABB(std::unique_ptr<GameObject>& obj1, std::unique_ptr<GameObject>& obj2)
{
    if (!obj1->HasComponent<BoxCollider>() || !obj2->HasComponent<BoxCollider>())
        return false;

    Transform& tr1 = *obj1->GetComponent<Transform>();
    Transform& tr2 = *obj2->GetComponent<Transform>();

    glm::vec3 aMin = tr1.position - obj1->GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 aMax = tr1.position + obj1->GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 bMin = tr2.position - obj2->GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 bMax = tr2.position + obj2->GetComponent<BoxCollider>()->size * 0.5f;

    return //衝突確認
    (//AとBの座標の最大点と最小点を比較して、重なっているかどうかを確認
    aMin.x <= bMax.x && aMax.x >= bMin.x &&

    aMin.y <= bMax.y && aMax.y >= bMin.y &&

    aMin.z <= bMax.z && aMax.z >= bMin.z
    );
}
void PhysicsSystem::ResolveAABB(std::unique_ptr<GameObject>& obj1, std::unique_ptr<GameObject>& obj2 , Rigidbody& rb)
{
    Transform& tr1 = *obj1->GetComponent<Transform>();
    Transform& tr2 = *obj2->GetComponent<Transform>();

    glm::vec3 aMin = tr1.position - obj1->GetComponent<BoxCollider>()->size * 0.5f;
    glm::vec3 aMax = tr1.position + obj1->GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 bMin = tr2.position - obj2->GetComponent<BoxCollider>()->size * 0.5f;
    glm::vec3 bMax = tr2.position + obj2->GetComponent<BoxCollider>()->size * 0.5f;

    float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);
    float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
    float overlapZ = std::min(aMax.z, bMax.z) - std::max(aMin.z, bMin.z);

    float minOverlap = std::min(overlapY, overlapX);
    minOverlap = std::min(minOverlap, overlapZ);

    // obj が上にいる時だけ
    if (minOverlap == overlapY)
    {
        if (tr1.position.y < tr2.position.y)
        {
            tr1.position.y -= overlapY;
        }
        else
        {
            tr1.position.y += overlapY;

            rb.isGrounded = true;
        }

        rb.velocity.y = 0.0f;
    }
    else if (minOverlap == overlapX)
    {
        if (tr1.position.x < tr2.position.x)
        {
            tr1.position.x -= overlapX;
        }
        else
        {
            tr1.position.x += overlapX;
        }

        rb.velocity.x = 0.0f;
    }
    else
    {
        if (tr1.position.z < tr2.position.z)
        {
            tr1.position.z -= overlapZ;
        }
        else
        {
            tr1.position.z += overlapZ;
        }

        rb.velocity.z = 0.0f;
    }
}
bool PhysicsSystem::CheckRayAABB(
    const Ray& ray,
    std::unique_ptr<GameObject>& obj,
    float& distance
)
{
    if (!obj->HasComponent<BoxCollider>())
        return false;


    glm::vec3 min =
        obj->GetComponent<Transform>()->position
        - obj->GetComponent<BoxCollider>()->size * 0.5f;

    glm::vec3 max =
        obj->GetComponent<Transform>()->position
        + obj->GetComponent<BoxCollider>()->size * 0.5f;

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
std::unique_ptr<GameObject>* PhysicsSystem::Raycast(
    Scene* scene,
    const Ray& ray)
{
    std::unique_ptr<GameObject>* result = nullptr;

    float nearest = FLT_MAX;

    for (auto& obj : scene->GetObjects())
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