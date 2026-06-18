#pragma once

#include "Window.h"
#include "EventInput.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "Scene.h"
#include <math.h>

class App
{
	public:
		App();
		int Run();
		void FixedUpdate(float dt);

	private:
		bool m_running;
		Window m_window;
		Timer m_timer;
		EventInput m_input;
		Shader m_defaultshader;
		Texture m_defaulttexture;
		Renderer m_renderer;
		Scene m_scene;
		Rigidbody m_rigidbody;
		Material m_defaultmaterial;
		Mesh m_mesh;
		PhysicsSystem m_physicsSystem;
		BoxCollider m_boxCollider;
};
