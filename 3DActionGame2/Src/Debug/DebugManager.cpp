#include "DebugManager.h"
#include "DxLib.h"


void DebugManager::AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc_)
{
#ifdef DEBUG
	logger.AddDebugLog(type_, message_, loc_);
#endif
}

void DebugManager::Stamp(Profiler::Type type_)
{

#ifdef DEBUG
	profiler.Stamp(type_);
#endif
}
void DebugManager::ResetTimes()
{
#ifdef DEBUG
	profiler.ResetTimes();
#endif
}

void DebugManager::Update()
{
#ifdef DEBUG
	profiler.Update();
#endif
}

void DebugManager::Render()
{
#ifdef DEBUG
	profiler.Render();

	if (CheckHitKey(KEY_INPUT_L))
	{
		logger.Render();
	}
#endif
}
