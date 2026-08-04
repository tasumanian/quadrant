#pragma once

#include "Component/Transform.h"
#include "Component/Component.h"
#include <string>
#include <vector>
#include <memory>

class GameObject
{
public:
	GameObject();

	template<typename T, typename... Args>
	T* AddComponent(Args&&... args);

	template<typename T>
	T* GetComponent();

	template<typename T>
	const std::vector<std::unique_ptr<Component>>& GetComponents() const;

	template<typename T>
	bool HasComponent();

	void SetParent(GameObject* parent);

    Transform transform;

    std::string name;

private :

    std::vector<std::unique_ptr<Component>> m_components;

};

#include "ResourceManager/GameObject.inl"