#pragma once

#include "Component/Transform.h"
#include "Component/Component.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

class GameObject
{
public:
	GameObject();

	GameObject(glm::vec3 pos);

	GameObject(glm::vec3 pos, glm::quat lotate);

	GameObject(glm::vec3 pos, glm::vec3 scale);

	GameObject(glm::vec3 pos, glm::quat lotate, glm::vec3 scale);

	template<typename T, typename... Args>
	T* AddComponent(Args&&... args);

	template<typename T>
	T* GetComponent();

	const std::vector<std::unique_ptr<Component>>& GetComponents() const;

	std::vector<std::unique_ptr<Component>>& GetComponents();
	template<typename T>
	bool HasComponent();

	void SetParent(std::unique_ptr<GameObject>& parent);

    std::string name;

private :

    std::vector<std::unique_ptr<Component>> m_components;

};

#include "ResourceManager/GameObject.inl"