#pragma once

#include <vector>

#include "Component/CameraComponent.h"
#include "ResourceManager/GameObject.h"

class GameObject;

class Scene
{
private:

    std::vector<GameObject> m_objects;

    CameraComponent* m_mainCamera;

public:

    Scene();

    std::vector<GameObject>& GetObjects();

    const std::vector<GameObject>& GetObjects() const;

    void AddObject(GameObject&& object);

    CameraComponent*
        GetMainCamera();

    void
        SetMainCamera(
            CameraComponent*);
};