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
#include "Frustum.h"
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

		// Stats of the last Draw call
		int GetDrawnCount() const { return m_drawnCount; }
		int GetCulledCount() const { return m_culledCount; }

	private:
		CameraSystem* m_cameraSystem = nullptr;
		Timer* m_timer = nullptr;
		Window* m_window = nullptr;
		Uint64 m_lastTicks = 0;

		Frustum m_frustum;
		int m_drawnCount = 0;
		int m_culledCount = 0;
};
