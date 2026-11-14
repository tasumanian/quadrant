#include "transform.h"

Transform::Transform()
{
    position = glm::vec3(0.0f);
    rotation = glm::identity<glm::quat>();
    scale = glm::vec3(1.0f);
    parent = nullptr;

    RegisterProperty(
        "Position",
        &position
    );

    RegisterProperty(
        "Rotation",
        &rotation
    );

    RegisterProperty(
        "Scale",
        &scale
    );
}
Transform::~Transform()
{
    SetParent(nullptr);

    for (Transform* child : children)
    {
        if (child != nullptr)
        {
            child->parent = nullptr;
        }
    }

    children.clear();
}
glm::vec3 Transform::Forward() const
{
    return rotation * glm::vec3(0, 0, -1);
}
glm::vec3 Transform::Back() const
{
    return rotation * glm::vec3(0, 0, 1);
}
glm::vec3 Transform::Right() const
{
    return rotation * glm::vec3(1, 0, 0);
}
glm::vec3 Transform::Left() const
{
    return rotation * glm::vec3(-1, 0, 0);
}
glm::vec3 Transform::Up() const
{
    return rotation * glm::vec3(0, 1, 0);
}
glm::vec3 Transform::Down() const
{
    return rotation * glm::vec3(0, -1, 0);
}
glm::mat4 Transform::GetWorldMatrix() const
{
    glm::mat4 localMatrix = GetLocalMatrix();

    if (parent != nullptr)
    {
        //親のトランスフォームを反映
        return parent->GetWorldMatrix() * localMatrix;
    }

    return localMatrix;
}
glm::mat4 Transform::GetLocalMatrix() const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model,position);

    model *= glm::toMat4(rotation);

    model = glm::scale( model,scale);

    return model;
}
glm::vec3 Transform::GetWorldPosition() const
{
    glm::mat4 worldMatrix = GetWorldMatrix();

    return glm::vec3(worldMatrix[3]);
}
void Transform::SetParent(Transform* newParent)
{
    if (parent == newParent || newParent == this|| 
        (newParent != nullptr && newParent->IsDescendantOf(this)))
        return;

    // 古い親から自分を削除
    if (parent != nullptr)
    {
        auto& siblings = parent->children;
        siblings.erase(
            std::remove(siblings.begin(), siblings.end(), this),
            siblings.end()
        );
    }

    parent = newParent;

    // 新しい親に追加
    if (parent != nullptr)
    {
        parent->children.push_back(this);
    }

}
bool Transform::IsDescendantOf(const Transform* transform) const
{
    const Transform* current = parent;

    while (current != nullptr)
    {
        if (current == transform)
            return true;

        current = current->parent;
    }

    return false;
}
void Transform::SetRotationEuler(const glm::vec3& euler)
{
    rotation = glm::quat(glm::radians(euler));
}
void Transform::SetWorldPosition(const glm::vec3& worldPosition)
{
    if (parent == nullptr)
    {
        position = worldPosition;
        return;
    }

    glm::mat4 parentWorld = parent->GetWorldMatrix();

    float determinant = glm::determinant(parentWorld);

    if (std::abs(determinant) < 1e-6f)
        return;

    glm::mat4 inverseParent = glm::inverse(parentWorld);

    position = glm::vec3(
        inverseParent * glm::vec4(worldPosition, 1.0f)
    );
}
glm::vec3 Transform::GetRotationEuler()
{
    return glm::degrees(glm::eulerAngles(rotation));
}
void Transform::Rotate(const glm::vec3& axis, float angle)
{
    if (glm::length(axis) <= 0.0001f)
        return;

    glm::quat deltaRotation =
        glm::angleAxis(
            glm::radians(angle),
            glm::normalize(axis)
        );

    rotation = glm::normalize(rotation * deltaRotation);
}