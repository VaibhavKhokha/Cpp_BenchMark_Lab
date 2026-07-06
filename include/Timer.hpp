#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StopTime;

public:
	void Start();
	void Stop();
	double elapsedMicroseconds();
};