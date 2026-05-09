#pragma once

#include <SDL3/SDL.h> 
#include "shader.h"
#include "mesh.h"
#include "transform.h"
#include "camera.h"
#include "stateinput.h"
#include <glad/glad.h>
#include <cmath>

class Window;

class Renderer
{
	public:
		bool Initialize(Window& window);
		void BeginFrame();
		void Draw();
		void EndFrame(Window& window);

	private:
		Shader m_shader;
		Mesh m_mesh;
		Transform m_transform;
		Camera m_camera;
		StateInput m_stateinput;
		Window* m_window = nullptr;
		Uint64 m_lastTicks = 0;
};
