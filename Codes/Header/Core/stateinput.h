#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <array>

class StateInput
{
    public:
        static void Update();

        static bool GetKey(SDL_Scancode key);

        static bool GetKeyDown(SDL_Scancode key);

        static bool GetKeyUp(SDL_Scancode key);

        static void ProcessEvent(const SDL_Event& event);

        static bool GetMouseButton(Uint8 button);

        static bool GetMouseButtonDown(Uint8 button);

        static bool GetMouseButtonUp(Uint8 button);

        static glm::vec2 GetMousePosition();

        static glm::vec2 GetMouseDelta();


    private:

        static std::array<bool, SDL_SCANCODE_COUNT> m_currentKeys;
        static std::array<bool, SDL_SCANCODE_COUNT> m_previousKeys;

        static std::array<bool, 8> m_currentMouse;
        static std::array<bool, 8> m_previousMouse;

        static glm::vec2 m_mousePosition;
        static glm::vec2 m_previousMousePosition;
};