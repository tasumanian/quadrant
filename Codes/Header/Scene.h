#pragma once

#include <vector>
#include <memory>

#include "CameraComponent.h"
#include "GameObject.h"

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

    void DestroyGameObject(GameObject* object);

    CameraComponent* GetMainCamera();

    void SetMainCamera(CameraComponent*);

    GameObject* CreateGameObject(const std::string& name);
};