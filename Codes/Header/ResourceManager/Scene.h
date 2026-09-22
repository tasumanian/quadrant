#pragma once

#include <vector>
#include <memory>

#include "Component/CameraComponent.h"
#include "ResourceManager/GameObject.h"

class GameObject;

class Scene
{
private:

    std::vector<std::unique_ptr<GameObject>> m_objects;

    CameraComponent* m_mainCamera;

public:

    Scene();

    std::vector<std::unique_ptr<GameObject>>& GetObjects();

    const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

    void AddObject(std::unique_ptr<GameObject> object);

    CameraComponent*
        GetMainCamera();

    void
        SetMainCamera(
            CameraComponent*);

    GameObject* CreateGameObject(
        const std::string& name);
};