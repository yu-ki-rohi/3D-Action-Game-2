#include "TimerManager.h"
#include "TimerBase.h"

std::shared_ptr<TimerBase> TimerManager::Add(std::shared_ptr<TimerBase> timer_)
{
	timers.push_back(timer_);
	return timer_;
}

void TimerManager::Remove(std::shared_ptr<TimerBase> timer_)
{
	timers.erase(std::remove(timers.begin(), timers.end(), timer_), timers.end());
}

void TimerManager::Erase()
{
	for (auto itr = timers.begin(); itr != timers.end(); )
	{
		if (*itr != nullptr && (*itr)->IsActive() == false)
		{
			itr = timers.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void TimerManager::EraseAll()
{
	timers.clear();
}

void TimerManager::Update(float elapsed_time_)
{
	for (auto& timer : timers)
	{
		timer->Update(elapsed_time_);
	}

	Erase();
}