#pragma once

#include "Core/Window.h"
#include "Core/EventInput.h"
#include "ResourceManager/Renderer.h"
#include "ResourceManager/PhysicsSystem.h"
#include "ResourceManager/Scene.h"
#include "Component/CharacterController.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Ray.h"
#include "Editor/Editor.h"
#include "ResourceManager/CameraSystem.h"
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
		CameraSystem m_cameraSystem;
		Timer m_timer;
		EventInput m_input;
		Renderer m_renderer;
		Scene m_scene;
		Rigidbody m_rigidbody;
		PhysicsSystem m_physicsSystem;
		BoxCollider m_boxCollider;
		CharacterController m_chacacterController;
		ResourceManager m_resourceManager;
		Editor m_editor;
};
