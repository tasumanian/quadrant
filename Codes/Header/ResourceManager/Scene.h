#pragma once

#include <vector>

#include "ResourceManager/GameObject.h"
#include "Component/CameraComponent.h"

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