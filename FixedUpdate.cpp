#include "FixedUpdate.h"

void FixedUpdate::FixedUpdateSet(float dt)
{
    auto& objects =
        m_renderer->GetObjects();

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
            GameObject& other
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
                m_physics->CheckAABB(
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