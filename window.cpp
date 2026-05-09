#include "window.h"
#include <windows.h>
#include <glad/glad.h>

Window::Window()
    : m_window(nullptr), m_glContext(nullptr)
{
}

Window::~Window()
{
    if (m_glContext)
    {
        SDL_GL_DestroyContext(m_glContext);
    }

    if (m_window)
    {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}
void Window::Present()
{
    SDL_GL_SwapWindow(m_window);
}

bool Window::Create(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext)
    {
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        return false;
    }

    return true;
}
SDL_Window* Window::GetSDLWindow() const
{
    return m_window;
}
