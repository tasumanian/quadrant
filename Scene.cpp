#include "Scene.h"

Scene::Scene()
{
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
    const GameObject& object
)
{
    m_objects.push_back(
        object
    );
}