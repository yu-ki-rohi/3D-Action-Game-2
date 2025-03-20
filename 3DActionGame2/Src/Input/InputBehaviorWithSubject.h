#pragma once
#include "InputBehavior.h"
#include "../Systems/Subject.h"

class InputBehaviorWithSubject : public InputBehavior
{
public:
	void AddObserver(std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(std::shared_ptr<ObserverBase> observer_);

	void ClearObservers();
protected:
	Subject subject;
};