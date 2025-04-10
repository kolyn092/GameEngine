#pragma once
#include <windows.h>

class ComponentTimer
{
public:
	ComponentTimer();
	~ComponentTimer();

	void Start();
	void End();

	double GetCurrentTimes();
	double GetPrevTime();
	double GetElapseTime();


private:
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;
	LARGE_INTEGER m_Frequency;

	double m_PrevTime;
	double m_CurrentTime;
};
