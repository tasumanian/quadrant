#pragma once

#include <cstdint>

class Timer
{
public:
	Timer();

	void Tick();
	float GetDeltaTime() const;

private:
	std::uint64_t m_lastCounter;
	float m_deltaTime;

};