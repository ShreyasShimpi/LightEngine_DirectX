#pragma once
// internal includes
#include <chrono>

class CTime
{
private:
	// variables
	std::chrono::steady_clock::time_point m_startTime;
	std::chrono::steady_clock::time_point m_endTime;
	long double m_deltaTime;
public:
	CTime();
	void Tick();

	// returns delta time(frame time) in milliseconds
	inline const long double DeltaTime() const { return m_deltaTime / 1000.0f; };
};
