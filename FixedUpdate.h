#pragma once
#include "Renderer.h"
#include "Physics.h"

class FixedUpdate
{
public:
	void FixedUpdateSet(float dt);

	Renderer* m_renderer;
	Physics* m_physics;

};