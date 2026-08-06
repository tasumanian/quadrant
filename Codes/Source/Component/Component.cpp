#include "Component/Component.h"
#include "ResourceManager/GameObject.h"

template<typename T>
void Component::RegisterProperty(
    const std::string& name,
    T* value)
{
    m_properties.emplace_back(
        std::make_unique<
        Property<T>>(
            name,
            value));
}
