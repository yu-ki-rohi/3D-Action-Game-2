#include "SystemTimeProcess.h"

float SystemTimeProcess::GetElapsedTime(LONGLONG& past_system_time_)
{
	LONGLONG current_system_time = GetNowHiPerformanceCount();
	float elapsed_time = (float)(current_system_time - past_system_time_) * 0.000001f;
	past_system_time_ = current_system_time;
	return elapsed_time;
}