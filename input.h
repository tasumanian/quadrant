#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

class Input
{
    public:
        void BeginFrame();
        void ProcessEvent(const SDL_Event& event);

        bool IsKeyPressed(SDL_Keycode key) const;
        bool IsKeyHeld(SDL_Keycode key) const;
        bool IsKeyReleased(SDL_Keycode key) const;

        bool QuitRequested() const;


    private:
        bool GetKeyState(const std::unordered_map<SDL_Keycode, bool>& map, SDL_Keycode key) const;

    private:
        bool m_quitRequested = false;
        std::unordered_map<SDL_Keycode, bool> m_currentKeys;
        std::unordered_map<SDL_Keycode, bool> m_previousKeys;

};
