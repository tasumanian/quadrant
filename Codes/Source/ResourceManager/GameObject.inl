#pragma once

template<typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args)
{
    auto component =
        std::make_unique<T>(
            std::forward<Args>(args)...);

    component->gameObject = this;

    T* ptr = component.get();

    m_components.emplace_back(
        std::move(component));

    return ptr;
}
template<typename T>
T* GameObject::GetComponent()
{
    for (auto& c : m_components)
    {
        if (auto ptr =
            dynamic_cast<T*>(c.get()))
        {
            return ptr;
        }
    }

    return nullptr;
}
template<typename T>
const std::vector<std::unique_ptr<Component>>&
GameObject::GetComponents() const
{
    return m_components;
}

template<typename T>
bool GameObject::HasComponent()
{
	if (GetComponent<T>() != nullptr)
	{
		return true;
	}
	return false;
}