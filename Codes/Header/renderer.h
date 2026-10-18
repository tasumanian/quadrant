#pragma once

#include <SDL3/SDL.h> 
#include "Shader.h"
#include "Texture.h"
#include "Timer.h"
#include "Components.h"
#include "StateInput.h"
#include "Scene.h"
#include "CharacterController.h"
#include "CameraSystem.h"
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
