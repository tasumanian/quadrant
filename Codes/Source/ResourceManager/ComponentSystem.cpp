#include "ResourceManager/ComponentSystem.h"
#include "ResourceManager/Scene.h"
#include "ResourceManager/GameObject.h"
#include "Component/Component.h"

void ComponentSystem::Awake(Scene& scene)
{
    for (auto& obj : scene.GetObjects())
    {
        //for (auto& component :
        //    obj.GetComponents())
        //{
        //    component->Update();
       // }
    }
}