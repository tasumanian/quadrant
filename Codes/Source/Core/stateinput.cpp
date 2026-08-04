#include "Core/StateInput.h"

std::array<bool, SDL_SCANCODE_COUNT> StateInput::m_currentKeys{};
std::array<bool, SDL_SCANCODE_COUNT> StateInput::m_previousKeys{};

std::array<bool, 8> StateInput::m_currentMouse{};
std::array<bool, 8> StateInput::m_previousMouse{};

glm::vec2 StateInput::m_mousePosition{};
glm::vec2 StateInput::m_previousMousePosition{};

void StateInput::Update()
{
    m_previousKeys = m_currentKeys;

    m_previousMouse = m_currentMouse;

    m_previousMousePosition =
        m_mousePosition;
}
void StateInput::ProcessEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:

        m_currentKeys[event.key.scancode] = true;

        break;

    case SDL_EVENT_KEY_UP:

        m_currentKeys[event.key.scancode] = false;

        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:

        m_currentMouse[event.button.button] = true;

        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:

        m_currentMouse[event.button.button] = false;

        break;

    case SDL_EVENT_MOUSE_MOTION:

        m_mousePosition.x =
            (float)event.motion.x;

        m_mousePosition.y =
            (float)event.motion.y;

        break;
    }
}

bool StateInput::GetKey(SDL_Scancode key)
{
    return m_currentKeys[key];
}
bool StateInput::GetKeyDown(SDL_Scancode key)
{
    return
        m_currentKeys[key] &&
        !m_previousKeys[key];
}
bool StateInput::GetKeyUp(SDL_Scancode key)
{
    return
        !m_currentKeys[key] &&
        m_previousKeys[key];
}
bool StateInput::GetMouseButton(Uint8 button)
{
    return m_currentMouse[button];
}
bool StateInput::GetMouseButtonDown(Uint8 button)
{
    return
        m_currentMouse[button] &&
        !m_previousMouse[button];
}
bool StateInput::GetMouseButtonUp(Uint8 button)
{
    return
        !m_currentMouse[button] &&
        m_previousMouse[button];
}
glm::vec2 StateInput::GetMousePosition()
{
    return m_mousePosition;
}
glm::vec2 StateInput::GetMouseDelta()
{
    return
        m_mousePosition -
        m_previousMousePosition;
}