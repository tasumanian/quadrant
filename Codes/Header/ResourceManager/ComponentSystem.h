#pragma once

class Scene;

class ComponentSystem
{
public:

    void Awake(Scene&);

    void Start(Scene&);

    void Update(Scene&, float);

    void FixedUpdate(Scene&, float);

    void LateUpdate(Scene&, float);
};