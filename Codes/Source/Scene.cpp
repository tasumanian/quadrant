#include "Scene.h"

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
void Scene::DestroyGameObject(
	GameObject* object)
{

	if (object == nullptr)
		return;

	auto it = std::find_if(
		m_objects.begin(),
		m_objects.end(),
		[object](const std::unique_ptr<GameObject>& obj)
		{
			return obj.get() == object;
		}
	);

	if (it != m_objects.end())
	{
		m_objects.erase(it);
	}
}