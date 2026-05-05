#pragma once

#include "shader.h"
#include "mesh.h"

class Window;

class Renderer
{
	public:
		bool Initialize();
		void BeginFrame();
		void Draw();
		void EndFrame(Window& window);

	private:
		Shader m_shader;
		Mesh m_mesh;
};
