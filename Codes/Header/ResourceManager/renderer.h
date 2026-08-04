#pragma once

#include <SDL3/SDL.h> 
#include "ResourceManager/Shader.h"
#include "ResourceManager/Texture.h"
#include "Core/Timer.h"
#include "ResourceManager/GameObject.h"
#include "Component/Components.h"
#include "Core/StateInput.h"
#include "ResourceManager/Scene.h"
#include "Component/CharacterController.h"
#include "ResourceManager/CameraSystem.h"
#include <glad/glad.h>
#include <cmath>
#include <vector>

class Window;

class Renderer
{
	public:
		bool Initialize(Window& window,Timer& timer, CameraSystem& cameraSystem);
		void BeginFrame();
		void Draw(Scene& scene);
		void EndFrame(Window& window);

	private:
		Rigidbody m_rigidbody;
		BoxCollider m_boxCollider;
		CameraSystem* m_cameraSystem;
		Timer* m_timer;
		StateInput m_stateinput;
		Window* m_window = nullptr;
		Uint64 m_lastTicks = 0;
		CharacterController m_characterController;
		
};
