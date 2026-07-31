#pragma once

#include <SDL3/SDL.h> 
#include "shader.h"
#include "camera.h"
#include "Texture.h"
#include "timer.h"
#include "GameObject.h"
#include "stateinput.h"
#include "BoxCollider.h"
#include "Scene.h"
#include "CharacterController.h"
#include <glad/glad.h>
#include <cmath>
#include <vector>

class Window;

class Renderer
{
	public:
		bool Initialize(Window& window,Timer& timer);
		void BeginFrame();
		void Draw(const Scene& scene,const Camera& camera);
		void EndFrame(Window& window);

	private:
		Rigidbody m_rigidbody;
		BoxCollider m_boxCollider;
		Timer* m_timer;
		std::vector<GameObject> m_objects; //シーケンスコンテナの一種
		StateInput m_stateinput;
		Window* m_window = nullptr;
		Uint64 m_lastTicks = 0;
		CharacterController m_characterController;
		
};
