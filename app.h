#pragma once

#include "Window.h"
#include "EventInput.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "Scene.h"
#include "CharacterController.h"
#include "ResourceManager.h"
#include "Editor.h"
#include <math.h>

class App
{
	public:
		App();
		int Run();
		void FixedUpdate(float dt);

	private:
		bool m_running;
		bool m_editorMode;
		Window m_window;
		Timer m_timer;
		EventInput m_input;
		Renderer m_renderer;
		Scene m_scene;
		Camera m_camera;
		Rigidbody m_rigidbody;
		PhysicsSystem m_physicsSystem;
		BoxCollider m_boxCollider;
		CharacterController m_chacacterController;
		ResourceManager m_resourceManager;
		Editor m_editor;
};
