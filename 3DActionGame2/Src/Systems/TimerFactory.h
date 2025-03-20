#pragma once
#include "TimerManager.h"
#include "Timer.h"
#include <memory>

class TimerFactory
{
public:
	template <class T>
	static std::shared_ptr<TimerBase> CreateTimer(float time_, T* obj_, void (T::* func_)())
	{
		return TimerManager::Instance().Add(std::make_shared<Timer<T>>(time_, obj_, func_));
	}
};