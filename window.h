#pragma once
#include <SDL3/SDL.h>

class Window
{
	public:
		Window();
		~Window();

		bool Create(const char* tutle, int width, int height);
		void Present();

	private:
		SDL_Window* m_window;
		SDL_GLContext m_glContext;
};