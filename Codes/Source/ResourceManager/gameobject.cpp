#include "ResourceManager/GameObject.h"
#include <type_traits>

GameObject::GameObject()
{
	name = "GameObject";
}
void GameObject::SetParent(
    GameObject* parent)
{
    transform.SetParent(
        &parent->transform);
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