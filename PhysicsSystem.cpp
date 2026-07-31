#include "PhysicsSystem.h"
#include <iostream>
void PhysicsSystem::Gravity(Scene* scene, float dt)
{
    auto& objects =
		scene->GetObjects();

    for (GameObject& obj:objects)
    {
        Rigidbody& rb = obj.rigidbody;
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

            // 衝突
            if (!CheckAABB(obj, other))
                continue;

            ResolveAABB(obj, other, rb);
         }   
    }
}
bool PhysicsSystem::CheckAABB(const GameObject& obj1, const GameObject& obj2)
{
    glm::vec3 aMin = obj1.transform.position - obj1.boxCollider.size * 0.5f;

    glm::vec3 aMax = obj1.transform.position + obj1.boxCollider.size * 0.5f;

    glm::vec3 bMin = obj2.transform.position - obj2.boxCollider.size * 0.5f;

    glm::vec3 bMax = obj2.transform.position + obj2.boxCollider.size * 0.5f;

    return //衝突確認
    (//AとBの座標の最大点と最小点を比較して、重なっているかどうかを確認
    aMin.x <= bMax.x && aMax.x >= bMin.x &&

    aMin.y <= bMax.y && aMax.y >= bMin.y &&

    aMin.z <= bMax.z && aMax.z >= bMin.z
    );
}
void PhysicsSystem::ResolveAABB(GameObject& obj1, GameObject& obj2 , Rigidbody& rb)
{
    glm::vec3 aMin = obj1.transform.position - obj1.boxCollider.size * 0.5f;
    glm::vec3 aMax = obj1.transform.position + obj1.boxCollider.size * 0.5f;

    glm::vec3 bMin = obj2.transform.position - obj2.boxCollider.size * 0.5f;
    glm::vec3 bMax = obj2.transform.position + obj2.boxCollider.size * 0.5f;

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