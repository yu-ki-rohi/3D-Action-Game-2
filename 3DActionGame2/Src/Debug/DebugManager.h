#pragma once
#include "../Common.h"

// デバッグ系の処理は最終的にはなるべくここにまとめたい

#ifdef DEBUG

#include<memory>

struct DebugLog
{
	enum class Type
	{
		Message,
		Warning,
		Error
	};

	std::unique_ptr<char> Message;

};

class DebugManager
{
public:
	
	void AddDebugLog(DebugLog::Type type_, char* message_);

	void Render();

private:


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
#endif