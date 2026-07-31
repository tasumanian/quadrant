#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	public:

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		Transform* parent;
		std::vector<Transform*> children;

		Transform();

		glm::vec3 Forward() const ;
		glm::vec3 Back() const;
		glm::vec3 Right() const;
		glm::vec3 Left() const;
		glm::vec3 Up() const;
		glm::vec3 Down() const;

		glm::mat4 GetWorldMatrix() const;
		glm::mat4 GetLocalMatrix() const;
		glm::mat4 GetMatrix() const;
		void SetParent(Transform* newParent);
};