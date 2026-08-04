#pragma once

class Scene;

class ComponentSystem
{
public:
    
    void Update(Scene&, float dt);

    void LateUpdate(Scene&, float dt);
};