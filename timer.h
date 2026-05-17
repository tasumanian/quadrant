#pragma once

#include <cstdint>

class Timer
{
public:
	Timer();

	void Tick();
	float GetDeltaTime() const;
	bool ShouldFixedUpdate();

	float GetFixedTimeStep() const;

private:
	float m_fixedTimeStep;
	float m_accumulator;
	float m_deltaTime;
	std::uint64_t m_lastCounter;

};