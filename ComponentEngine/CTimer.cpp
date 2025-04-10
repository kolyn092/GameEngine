#include "pch.h"
#include "CTimer.h"

ComponentTimer::ComponentTimer() :
	m_BeginTime(), m_EndTime(), m_Frequency(), m_CurrentTime(0), m_PrevTime(0)
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_EndTime);
}

ComponentTimer::~ComponentTimer()
{

}

void ComponentTimer::Start()
{
	QueryPerformanceCounter(&m_BeginTime);
}

void ComponentTimer::End()
{
	m_PrevTime = (float)(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_Frequency.QuadPart;
	QueryPerformanceCounter(&m_EndTime);
	m_CurrentTime = (float)(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_Frequency.QuadPart;
}

double ComponentTimer::GetCurrentTimes()
{
	return m_CurrentTime;
}


double ComponentTimer::GetPrevTime()
{
	return m_PrevTime;
}

double ComponentTimer::GetElapseTime()
{
	return (float)(m_CurrentTime - m_PrevTime);
}


