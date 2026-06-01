#pragma once

#include "Window.h"
#include "EventInput.h"
#include "Renderer.h"
#include "Physics.h"
#include "FixedUpdate.h"
#include <math.h>

class App
{
	public:
		App();
		int Run();

	private:
		bool m_running;
		Window m_window;
		Timer m_timer;
		EventInput m_input;
		Renderer m_renderer;
		Physics m_physics;
		FixedUpdate m_fixedUpdate;
};
