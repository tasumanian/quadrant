#include "stateinput.h"

#include <SDL3/SDL.h>

bool StateInput::s_quit = false;

int StateInput::s_mouseDeltaX = 0;
int StateInput::s_mouseDeltaY = 0;

void StateInput::Update()
{
    SDL_Event event;
    s_mouseDeltaX = 0;
    s_mouseDeltaY = 0;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            s_quit = true;
        }
    }
    UpdateMouse();
}

void StateInput::UpdateMouse()
{
    float x;
    float y;

    SDL_GetRelativeMouseState(&x, &y);

    s_mouseDeltaX = (int)x;
    s_mouseDeltaY = (int)y;
}

bool StateInput::IsKeyDown(int scancode)
{
    const bool* keyboard =
        SDL_GetKeyboardState(nullptr);

    return keyboard[scancode];
}

bool StateInput::QuitRequested()
{
    return s_quit;
}
int StateInput::GetMouseDeltaX()
{
    return s_mouseDeltaX;
}

int StateInput::GetMouseDeltaY()
{
    return s_mouseDeltaY;
}