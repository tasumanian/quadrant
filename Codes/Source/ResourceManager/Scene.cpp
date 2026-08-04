#include "ResourceManager/Scene.h"

Scene::Scene()
{
	m_mainCamera = nullptr;

}

std::vector<GameObject>&
Scene::GetObjects()
{
    return m_objects;
}

const std::vector<GameObject>&
Scene::GetObjects() const
{
    return m_objects;
}

void Scene::AddObject(
    GameObject&& object
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