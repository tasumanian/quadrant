#include "ResourceManager/Scene.h"

Scene::Scene()
{
	m_mainCamera = nullptr;
	// temporary no-op signature update

}

std::vector<std::unique_ptr<GameObject>>&
Scene::GetObjects()
{
	return m_objects;
}

const std::vector<std::unique_ptr<GameObject>>&
Scene::GetObjects() const
{
	return m_objects;
}

void Scene::AddObject(
	std::unique_ptr<GameObject> object
)
{
	m_objects.push_back(std::move(object));
}
void Scene::SetMainCamera(
	CameraComponent* camera
)
{
	m_mainCamera = camera;
}
CameraComponent* Scene::GetMainCamera()
{
	return m_mainCamera;
}
GameObject* Scene::CreateGameObject(
	const std::string& name)
{
	auto object =
		std::make_unique<GameObject>();

	object->name = name;

	GameObject* ptr =
		object.get();

	m_objects.emplace_back(
		std::move(object));

	return ptr;
}