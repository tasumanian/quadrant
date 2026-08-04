#include "Core/EventInput.h"

void EventInput::BeginFrame()
{
    m_quitRequested = false;
    m_previousKeys = m_currentKeys;
}

//入力イベントの監視
void EventInput::ProcessEvent(const SDL_Event& event)
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

bool EventInput::GetKeyState(const std::unordered_map<SDL_Keycode, bool>& map, SDL_Keycode key) const
{
    auto it = map.find(key);
    if (it != map.end())
    {
        return it->second;
    }

    return false;
}

//押し込み
bool EventInput::IsKeyPressed(SDL_Keycode key) const
{
    return GetKeyState(m_currentKeys, key) && !GetKeyState(m_previousKeys, key);
}

//長押し
bool EventInput::IsKeyHeld(SDL_Keycode key) const
{
    return GetKeyState(m_currentKeys, key);
}

//離す
bool EventInput::IsKeyReleased(SDL_Keycode key) const
{
    return !GetKeyState(m_currentKeys, key) && GetKeyState(m_previousKeys, key);
}

bool EventInput::QuitRequested() const
{
    return m_quitRequested;
}