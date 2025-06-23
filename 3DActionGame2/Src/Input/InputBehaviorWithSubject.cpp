#include "InputBehaviorWithSubject.h"

void InputBehaviorWithSubject::AddObserver(InputManager::Map map_, std::weak_ptr<ObserverBase> observer_)
{
	subjects[map_].AddObserver(observer_);
}

void InputBehaviorWithSubject::RemoveObserver(InputManager::Map map_, std::shared_ptr<ObserverBase> observer_)
{
	subjects[map_].RemoveObserver(observer_);
}

void InputBehaviorWithSubject::ClearObservers()
{
	for (auto& subject : subjects)
	{
		subject.second.Clear();
	}
}