#include "DebugMessenger.h"
#include "DebugManager.h"

void DebugMessenger::Log(const char* message_, const std::source_location& loc_)
{
	DebugManager::Instance().AddDebugLog(DebugLog::Type::Message, message_, loc_);
}

void DebugMessenger::LogWarning(const char* message_, const std::source_location& loc_)
{
	DebugManager::Instance().AddDebugLog(DebugLog::Type::Warning, message_, loc_);
}

void DebugMessenger::LogError(const char* message_, const std::source_location& loc_)
{
	DebugManager::Instance().AddDebugLog(DebugLog::Type::Error, message_, loc_);
}