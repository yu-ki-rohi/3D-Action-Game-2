#include "InputBehaviorWithSubject.h"

void InputBehaviorWithSubject::AddObserver(std::weak_ptr<ObserverBase> observer_)
{
	subject.AddObserver(observer_);
}

void InputBehaviorWithSubject::RemoveObserver(std::shared_ptr<ObserverBase> observer_)
{
	subject.RemoveObserver(observer_);
}

void InputBehaviorWithSubject::ClearObservers()
{
	subject.Clear();
}