#pragma once

#include <glm/glm.hpp>

class GameObject;

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