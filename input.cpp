#include "input.h"

void Input::BeginFrame()
{
    m_quitRequested = false;
    m_previousKeys = m_currentKeys;
}

//入力イベントの監視
void Input::ProcessEvent(const SDL_Event& event) 
{
    if (event.type == SDL_EVENT_QUIT)
    {
        m_quitRequested = true;
    }

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        m_currentKeys[event.key.key] = true;
    }

    if (event.type == SDL_EVENT_KEY_UP)
    {
        m_currentKeys[event.key.key] = false;
    }
}

bool Input::GetKeyState(const std::unordered_map<SDL_Keycode, bool>& map, SDL_Keycode key) const
{
    auto it = map.find(key);
    if (it != map.end())
    {
        return it->second;
    }

    return false;
}

//押し込み
bool Input::IsKeyPressed(SDL_Keycode key) const
{
    return GetKeyState(m_currentKeys, key) && !GetKeyState(m_previousKeys, key);
}

//長押し
bool Input::IsKeyHeld(SDL_Keycode key) const
{
    return GetKeyState(m_currentKeys, key);
}

//離す
bool Input::IsKeyReleased(SDL_Keycode key) const
{
    return !GetKeyState(m_currentKeys, key) && GetKeyState(m_previousKeys, key);
}

bool Input::QuitRequested() const
{
    return m_quitRequested;
}