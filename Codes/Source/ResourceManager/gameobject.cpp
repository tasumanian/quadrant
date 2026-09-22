#include "ResourceManager/GameObject.h"
#include <type_traits>

GameObject::GameObject()
{
	name = "GameObject";

    AddComponent<Transform>();
}
GameObject::GameObject(glm::vec3 pos)
{
    name = "GameObject";

    Transform* tr = AddComponent<Transform>();

    tr->position = pos;

}
GameObject::GameObject(glm::vec3 pos, glm::quat lotate)
{
    name = "GameObject";

    Transform* tr = AddComponent<Transform>();

    tr->position = pos;
    tr->rotation = lotate;
}
GameObject::GameObject(glm::vec3 pos, glm::vec3 scale)
{
    name = "GameObject";

    Transform* tr = AddComponent<Transform>();

    tr->position = pos;
    tr->scale = scale;
}
GameObject::GameObject(glm::vec3 pos, glm::quat lotate, glm::vec3 scale)
{
    name = "GameObject";

    Transform* tr = AddComponent<Transform>();

    tr->position = pos;
    tr->rotation = lotate;
    tr->scale = scale;
}

void GameObject::SetParent(
    std::unique_ptr<GameObject>& parent)
{
    GetComponent<Transform>()->SetParent(
        parent->GetComponent<Transform>());
}
const std::vector<std::unique_ptr<Component>>&
GameObject::GetComponents() const
{
    return m_components;
}
std::vector<std::unique_ptr<Component>>&
GameObject::GetComponents()
{
    return m_components;
}