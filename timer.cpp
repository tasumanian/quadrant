#include "timer.h"
#include <SDL3/SDL.h>

Timer::Timer()
	: m_lastCounter(SDL_GetPerformanceCounter())
	, m_deltaTime(0.0f)
	, m_fixedTimeStep(1.0f / 60.0f)
	, m_accumulator(0.0f)
{
}

void Timer::Tick()
{
	std::uint64_t current = SDL_GetPerformanceCounter();
	std::uint64_t frequency = SDL_GetPerformanceFrequency();

	//前フレームとの差
	m_deltaTime = static_cast<float>(current - m_lastCounter) / static_cast<float>(frequency);
	SDL_Log("Delta Time: %f seconds", m_deltaTime);
	m_lastCounter = current;

	m_accumulator += m_deltaTime;
}

float Timer::GetDeltaTime() const
{
	return m_deltaTime;
}
bool Timer::ShouldFixedUpdate()
{
	if (
		m_accumulator
		>=
		m_fixedTimeStep
		)
	{
		m_accumulator -=
			m_fixedTimeStep;

		return true;
	}

	return false;
}
float Timer::GetFixedTimeStep() const
{
	return m_fixedTimeStep;
}