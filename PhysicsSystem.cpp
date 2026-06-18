#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::Gravity(Scene* scene, float dt)
{
    auto& objects =
		scene->GetObjects();

    for (
        GameObject& obj
        :
        objects
        )
    {

        Rigidbody rb =
            obj.rigidbody;
		rb.isGrounded = false;

        // 重力
        if (rb.useGravity)
        {
            rb.AdaptGravity(dt);
            obj.transform.position +=
                rb.velocity * dt;
        }
        // 衝突判定
        for (
            GameObject other
            :
            objects
            )
        {
            // 自分自身除外
            if (&obj == &other)
            {
                continue;
            }

            // 衝突
            if (
                CheckAABB(
                    obj,
                    other
                )
                )
            {
                glm::vec3 aMin =
                    obj.transform.position
                    -
                    obj.boxCollider.size * 0.5f;

                glm::vec3 aMax =
                    obj.transform.position
                    +
                    obj.boxCollider.size * 0.5f;

                glm::vec3 bMin =
                    other.transform.position
                    -
                    other.boxCollider.size * 0.5f;

                glm::vec3 bMax =
                    other.transform.position
                    +
                    other.boxCollider.size * 0.5f;

                float overlapY =
                    std::min(aMax.y, bMax.y)
                    -
                    std::max(aMin.y, bMin.y);

                // obj が上にいる時だけ
                if (
                    obj.transform.position.y >
                    other.transform.position.y
                    )
                {
                    obj.transform.position.y +=
                        overlapY;

                    rb.velocity.y = 0.0f;

                    rb.isGrounded = true;
                }
            }
        }
    }
}
bool PhysicsSystem::CheckAABB(const GameObject& obj1, const GameObject& obj2)
{
    glm::vec3 aMin =
        obj1.transform.position
        -
        obj1.boxCollider.size * 0.5f;

    glm::vec3 aMax =
        obj1.transform.position
        +
        obj1.boxCollider.size * 0.5f;

    glm::vec3 bMin =
        obj2.transform.position
        -
        obj2.boxCollider.size * 0.5f;

    glm::vec3 bMax =
        obj2.transform.position
        +
        obj2.boxCollider.size * 0.5f;

    return //衝突確認
           //AとBの座標の最大点と最小点を比較して、重なっているかどうかを確認
        (
            aMin.x <= bMax.x &&
            aMax.x >= bMin.x &&

            aMin.y <= bMax.y &&
            aMax.y >= bMin.y &&

            aMin.z <= bMax.z &&
            aMax.z >= bMin.z
            );
}