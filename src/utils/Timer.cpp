#include "Timer.hpp"

void Timer::Start()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
	m_StopTime = std::chrono::high_resolution_clock::now();
}

double Timer::elapsedMicroseconds()
{
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_StopTime - m_StartTime);
	
	return duration.count();
}