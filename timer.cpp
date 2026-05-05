#include "timer.h"
#include <SDL3/SDL.h>

Timer::Timer()
	: m_lastCounter(SDL_GetPerformanceCounter())
	, m_deltaTime(0.0f)
{
}

void Timer::Tick()
{
	std::uint64_t current = SDL_GetPerformanceCounter();
	std::uint64_t frequency = SDL_GetPerformanceFrequency();

	//前フレームとの差
	m_deltaTime = static_cast<float>(current - m_lastCounter) / static_cast<float>(frequency);
	
	m_lastCounter = current;
}

float Timer::GetDeltaTime() const
{
	return m_deltaTime;
}