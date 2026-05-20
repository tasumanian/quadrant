#include "Physics.h"

bool Physics::CheckAABB(const GameObject& obj1, const GameObject& obj2)
{
    glm::vec3 aMin =
        obj1.transform.position
        -
        obj1.boxCollider->size * 0.5f;

    glm::vec3 aMax =
        obj1.transform.position
        +
        obj1.boxCollider->size * 0.5f;

    glm::vec3 bMin =
        obj2.transform.position
        -
        obj2.boxCollider->size * 0.5f;

    glm::vec3 bMax =
        obj2.transform.position
        +
        obj2.boxCollider->size * 0.5f;

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