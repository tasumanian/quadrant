#include "ResourceManager/ComponentSystem.h"
#include "ResourceManager/Scene.h"
#include "ResourceManager/GameObject.h"
#include "Component/Component.h"

void ComponentSystem::Update(
    Scene& scene,
    float dt)
{
    for (GameObject& obj : scene.GetObjects())
    {
        for (auto& component : obj.GetComponents())
        {
            if (!component->enabled)
                continue;

            if (!component->m_awaked)
            {
                component->Awake();
                component->m_awaked = true;
            }

            if (!component->m_started)
            {
                component->Start();
                component->m_started = true;
            }

            component->Update(dt);
        }
    }
}
void ComponentSystem::LateUpdate(
    Scene& scene,
    float dt)
{
    for (GameObject& obj : scene.GetObjects())
    {
        for (auto& component : obj.GetComponents())
        {
            if (!component->enabled)
                continue;

            component->LateUpdate(dt);
        }
    }
}