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