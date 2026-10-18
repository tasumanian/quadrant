#pragma once

class Scene;

class ComponentSystem
{
public:
    
	void Awake(Scene&);

	void Start(Scene&);


    void Update(Scene&, float dt);

	void FixedUpdate(Scene&, float dt);
    
	void LateUpdate(Scene&, float dt);
};