#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Component.h"

class Transform : public Component
{
	public:

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		Transform* parent;
		std::vector<Transform*> children;

		Transform();
		~Transform();

		const char* GetComponentName() const override
		{
			return "Transform";
		}

		bool IsDescendantOf(const Transform* transform) const;

		glm::vec3 Forward() const ;
		glm::vec3 Back() const;
		glm::vec3 Right() const;
		glm::vec3 Left() const;
		glm::vec3 Up() const;
		glm::vec3 Down() const;

		glm::mat4 GetWorldMatrix() const;
		glm::mat4 GetLocalMatrix() const;
		void SetParent(Transform* newParent);
};