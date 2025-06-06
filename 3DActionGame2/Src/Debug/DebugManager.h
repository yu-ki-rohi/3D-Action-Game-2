#pragma once
#include "../Common.h"

class DebugManager
{
public:


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