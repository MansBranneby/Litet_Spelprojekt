#include "Clock.h"

Clock::Clock()
{
	m_dt = 0;
	m_timeInSec = 0;
	m_currTime.QuadPart = 0;
	m_delta.QuadPart = 0;

	QueryPerformanceFrequency(&m_clockFreq);
	QueryPerformanceCounter(&m_startTime);
}

void Clock::resetStartTimer()
{
	m_startTime = m_currTime;
}

void Clock::resetSecTimer()
{
	m_timeInSec = 0;
}

void Clock::calcDeltaTime()
{
	QueryPerformanceCounter(&m_currTime);
	m_delta.QuadPart = m_currTime.QuadPart - m_startTime.QuadPart;
	m_dt = (float)m_delta.QuadPart / m_clockFreq.QuadPart;
}

float Clock::getDeltaTime()
{
	return m_dt;
}

float Clock::getTimeInSec()
{
	return m_timeInSec += m_dt;
}

