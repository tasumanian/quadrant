#pragma once

#include <vector>

#include "GameObject.h"

class Scene
{
private:

    std::vector<GameObject> m_objects;

public:

    Scene();

    std::vector<GameObject>&
        GetObjects();

    const std::vector<GameObject>&
        GetObjects() const;

    void AddObject(
        const GameObject& object
    );
};