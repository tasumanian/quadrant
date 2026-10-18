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
    if (parent == nullptr)
    {
        return GetLocalMatrix();
    }

    return
        parent->GetWorldMatrix() * GetLocalMatrix();                                                                                                              
}
glm::mat4 Transform::GetLocalMatrix() const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model,position);

    model *= glm::toMat4(rotation);

    model = glm::scale( model,scale);

    return model;
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
