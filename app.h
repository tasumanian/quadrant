#pragma once

#include "window.h"
#include "timer.h"
#include "input.h"
#include "renderer.h"

class App
{
	public:
		App();
		int Run();

	private:
		bool m_running;
		Window m_window;
		Timer m_timer;
		Input m_input;
		Renderer m_renderer;
};
