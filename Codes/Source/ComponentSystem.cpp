#include "ComponentSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"

void ComponentSystem::Awake(
	Scene& scene)
{
	for (auto& obj : scene.GetObjects())
	{
        GameObject& object = *obj;

		for (auto& com : object.GetComponents())
		{
            Component& component = *com;

			if (!component.m_awaked)
			{
				component.Awake();
				component.m_awaked = true;
			}
		}
	}
}
void ComponentSystem::Start(
	Scene& scene)
{
	for (auto& obj : scene.GetObjects())
	{
		GameObject& object = *obj;

		for (auto& com : object.GetComponents())
		{
			Component& component = *com;

			if (!component.m_started)
			{
				component.Start();
				component.m_started = true;
			}
		}
	}
}


void ComponentSystem::Update(
    Scene& scene,
    float dt)
{
	for (auto& obj : scene.GetObjects())
	{
		GameObject& object = *obj;

		for (auto& com : object.GetComponents())
		{
			Component& component = *com;

			if (!component.enabled)
				continue;

            component.Update(dt);
        }
    }
}
void ComponentSystem::FixedUpdate(
	Scene& scene,
	float dt)
{
	for (auto& obj : scene.GetObjects())
	{
		GameObject& object = *obj;
		for (auto& com : object.GetComponents())
		{
			Component& component = *com;

			if (!component.enabled)
				continue;

			component.FixedUpdate(dt);
		}
	}
}


void ComponentSystem::LateUpdate(
    Scene& scene,
    float dt)
{
    for (auto& obj : scene.GetObjects())
    {
		GameObject& object = *obj;

        for (auto& com : object.GetComponents())
        {
            Component& component = *com;

            if (!component.enabled)
                continue;

            component.LateUpdate(dt);
        }
    }
}