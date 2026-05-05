#pragma once

#include "window.h"
#include "timer.h"
#include "input.h"

class App
{
	public:int Run();

	private:
		bool m_running;
		Window m_window;
		Timer m_timer;
		Input m_input;
};
