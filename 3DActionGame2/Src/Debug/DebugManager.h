#pragma once
#include "../Common.h"

// デバッグ系の処理は最終的にはなるべくここにまとめたい
// todo LogやAssertを画面に出力する機能の追加

#include<memory>
#include "DebugLogger.h"
#include "Profiler.h"

class DebugManager
{
public:
	void AddDebugLog(DebugLog::Type type_, const char* message_, const std::source_location& loc = std::source_location::current());

	void Stamp(Profiler::Type type_);
	void ResetTimes();

	void Update();
	void Render();

private:
#ifdef DEBUG
	Profiler profiler;
	DebugLogger logger;
#endif
#pragma region シングルトン登録
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