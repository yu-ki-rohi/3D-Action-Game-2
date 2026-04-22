#pragma once
#include <memory>
#include <vector>

class TimerBase;

class TimerManager
{
public:
	// Timerの追加(待機列に追加し、実際の追加は次の更新時から)
	std::shared_ptr<TimerBase> Add(std::shared_ptr<TimerBase> timer_);

	// 待機列から実行列に移動
	void Entry();
	void Update(float elapsed_time_);

	// 実行済みのTimerの除去
	void Erase();
	// 全てのTimerを消去
	void EraseAll();

private:
	std::vector<std::shared_ptr<TimerBase>> timers;
	std::vector<std::shared_ptr<TimerBase>> pendingStartTimers;

#pragma region シングルトン登録
public:
	static TimerManager& Instance()
	{
		static TimerManager instance;
		return instance;
	}

	TimerManager(const TimerManager&) = delete;
	TimerManager& operator=(const TimerManager&) = delete;
	TimerManager(TimerManager&&) = delete;
	TimerManager& operator=(TimerManager&&) = delete;

private:
	TimerManager() = default;
	~TimerManager() = default;
#pragma endregion
};