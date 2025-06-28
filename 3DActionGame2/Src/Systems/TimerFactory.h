#pragma once
#include "TimerManager.h"
#include "Timer.h"
#include <memory>

class TimerFactory
{
public:
	template <class T, class V>
	static std::shared_ptr<TimerBase> CreateTimer(float time_, std::shared_ptr<V> existence_, T* obj_, void (T::* func_)())
	{
		return TimerManager::Instance().Add(std::make_shared<Timer<T,V>>(time_, existence_, obj_, func_));
	}
};