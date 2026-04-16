#pragma once

#include <source_location>

static class DebugMessenger
{
public:
	static void Log(const char* message_, const std::source_location& loc_ = std::source_location::current());
	static void LogWarning(const char* message_, const std::source_location& loc_ = std::source_location::current());
	static void LogError(const char* message_, const std::source_location& loc_ = std::source_location::current());
};