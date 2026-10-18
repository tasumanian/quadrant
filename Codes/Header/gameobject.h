#pragma once

#include "Transform.h"
#include "Component.h"
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

	void SetParent(GameObject* parent);

	Transform* GetTransform();

    std::string name;

private :

    std::vector<std::unique_ptr<Component>> m_components;

};

#include "GameObject.inl"