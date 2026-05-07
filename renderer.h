#pragma once

#include "shader.h"
#include "mesh.h"
#include "transform.h"
#include "camera.h"

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
		Transform m_transform;
		Camera m_camera;
};
