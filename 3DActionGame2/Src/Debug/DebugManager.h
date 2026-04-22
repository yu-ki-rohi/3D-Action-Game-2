#pragma once
#include "../Common.h"

#include "DebugLogger.h"
#include "Profiler.h"

class DebugManager
{
public:
	void AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc = std::source_location::current());

	void Stamp(Profiler::Type type_);
	void ResetTimes();
	void UpdateProfiler();

	void Update(float elapsed_time_);
	void Render();



private:
#ifdef DEBUG
	Profiler profiler;
	DebugLogger logger;
#endif
#pragma region ƒVƒ“ƒOƒ‹ƒgƒ““o˜^
public:
	static DebugManager& Instance()
	{
		static DebugManager instance;
		return instance;
	}

	DebugManager(const DebugManager&) = delete;
	DebugManager& operator=(const DebugManager&) = delete;
	DebugManager(DebugManager&&) = delete;
	DebugManager& operator=(DebugManager&&) = delete;

private:
	DebugManager() = default;
	~DebugManager() = default;
#pragma endregion

};