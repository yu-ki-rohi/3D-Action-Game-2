#pragma once
#include <memory>
#include <vector>

class TimerBase;

class TimerManager
{
public:
	std::shared_ptr<TimerBase> Add(std::shared_ptr<TimerBase> timer_);
	void Remove(std::shared_ptr<TimerBase> timer_);

	void Update(float elapsed_time_);

	void Erase();
	void EraseAll();
private:
	std::vector<std::shared_ptr<TimerBase>> timers;

#pragma region ƒVƒ“ƒOƒ‹ƒgƒ““o˜^
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